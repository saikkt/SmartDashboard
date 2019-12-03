//SmartScanner Implementation
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <regex>
#include "SmartScanner.h"

//constants
#define RESPONSE_BUFFER_LEN 1024
#define SSDP_MULTICAST "239.255.255.250"
#define SSDP_PORT 1900

using namespace std;

  int SmartScanner ::  scanLAN()
  {
    int ipIndex = 0;//key for map
    std::string finalIP; //IP address of the device
    int sock; //integer variable to store returned value by socket
    size_t ret; //variable to store size of returned response from device
    unsigned int socklen;
    struct sockaddr_in sockname;
    struct sockaddr clientsock;
    struct hostent *hostname;

    //discovery message
    char data[] =
        "M-SEARCH * HTTP/1.1\r\n"\
        "HOST: 239.255.255.250:1900\r\n"\
        "MAN: \"ssdp:discover\"\r\n"\
        "MX: 5\r\n"\
        "ST: upnp:rootdevice\r\n\r\n";

    char buffer[RESPONSE_BUFFER_LEN]; //character array to store response
    char *bufferp; //pointer to char array
    unsigned int len = RESPONSE_BUFFER_LEN;    
    fd_set fds; //variable to store function descriptor
    struct timeval timeout; //structure to to define time interval

    hostname = gethostbyname(SSDP_MULTICAST);
    hostname->h_addrtype = AF_INET;

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) //check if socket is created succesfully
    {
        printf("err: socket() failed");
        return -1;
    }

    memset((char *)&sockname, 0, sizeof(struct sockaddr_in));
    sockname.sin_family = AF_INET; //set IPv4 
    sockname.sin_port = htons(SSDP_PORT); //set port
    sockname.sin_addr.s_addr = *((unsigned long *)(hostname->h_addr_list[0]));

    ret = sendto(sock, data, strlen(data), 0, (struct sockaddr *)&sockname,
                 sizeof(struct sockaddr_in)); //send data
    if(ret != strlen(data))//check if discovery message and returned length is same
    {
        printf("err:sendto");
        return -1;
    }

    /* Get response */
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    while(select(sock + 1, &fds, NULL, NULL, &timeout) > 0) //select each device
    {
        if(FD_ISSET(sock, &fds))
        {
            socklen = sizeof(clientsock);
            if((len = recvfrom(sock, buffer, len, 0, &clientsock, &socklen)) == (size_t)-1) //recieve data
            {
                printf("err: recvfrom");
                return -1;
            }

            buffer[len] = '\0';
	    
            /* Check the HTTP response code */
            if(strncmp(buffer, "HTTP/1.1 200 OK", 12) != 0)
            {
                printf("err: ssdp parsing ");
                return -1;
            }

	ipIndex++; //device found - increase ipIndex by 1 
    //manipulate buffer character array to extract locattion header
	string temp; //temporary string variable to store character buffer array - response message
	temp = buffer; //copy array into string
	char ip[temp.size()]; //character array to store device address
    string temp2;
	size_t length; //size_t variable to store length of the returned device message
	length = temp.copy(ip,temp.size(),temp.find("http://")); //find http in device response message andd copy into character array
	ip[length] = '\0';
	string ip2;
	ip2 = ip;//store character array into string
	int index = ip2.find("\n"); //find first end of line in string
	char deviceIP[index]; //new character array of length ip address of device
	int index2 = temp.copy(deviceIP,index,temp.find("http://"));//copy from http to end of ip address
	deviceIP[index-1] = '\0';
	finalIP = deviceIP; //set finalIP
	this->ipAddresses.insert(pair<int,string>(ipIndex,finalIP)); //store ip and index in map
        }
        else
        {
            printf("err: no ssdp answer");
        }
    }
    close(sock);//close socket
    return 1;
  }



 std::map<int,string> SmartScanner :: getipAddresses() //function to return ipAddresses map
  {
      return ipAddresses;
  } 
