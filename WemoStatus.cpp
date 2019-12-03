//WemoStatus Implemetation
#include "WemoStatus.h"
#include <curl/curl.h>
#include "tinyxml2.h"
#include <iostream>

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void WemoStatus :: getWemoStatus(string URL) //prints status of wemo device
{
    int deviceStatus; //variable to store binary state od device
    string readBuffer; //buffer to store response from wemo smart switch
    CURL* hnd = curl_easy_init(); //initialize curl easy handle
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST"); //set method type
	curl_easy_setopt(hnd, CURLOPT_URL, URL.c_str()); //set url

	struct curl_slist* headers = NULL; //initialize headers
	headers = curl_slist_append(headers, "SOAPACTION: \"urn:Belkin:service:basicevent:1#GetBinaryState\"");//append headers
	headers = curl_slist_append(headers, "Content-Type: text/xml;"); //append headers
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers); //pass in headers
    //Soap request body
	string body ="<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                 <s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\
                 <s:Body>\
                 <u:GetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\">\
                 </u:GetBinaryState>\
                 </s:Body>\
                 </s:Envelope>";
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, body.c_str()); //pass in body
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);//call writecallback function to write data into string 
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer); //write received data into string
	curl_easy_perform(hnd); //send request
    //std::cout<<readBuffer; //debugging purpose

    tinyxml2::XMLDocument xml_doc; //initialize xml document
    tinyxml2::XMLError eResult = xml_doc.Parse(readBuffer.c_str()); //parse xml document
    if (eResult != tinyxml2::XML_SUCCESS)  //check if parsing went wrong
    {
        std::cout<<"Error in parsing XML\n";
        return;
    }
    tinyxml2::XMLElement* deviceDetails = xml_doc.FirstChildElement("s:Envelope")->FirstChildElement("s:Body")->FirstChildElement("u:GetBinaryStateResponse"); //traverse to GetBinaryState
    deviceStatus = (int) deviceDetails->FirstChildElement("BinaryState")->IntText(); //copy binary state into variable

    if(deviceStatus==0) //check if device is off
    {
        cout<<"\tDevice Status :: OFF\n\n";
    }

    else if(deviceStatus==1) //check if device is on
    {
        cout<<"\tDevice Status :: ON\n\n";
    }

    else
    {
        cout<<"\tError in getting device status\n";
    }
    
    

}