#include "ToggleWemo.h"
#include <curl/curl.h>
#include "tinyxml2.h"
#include <iostream>


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void ToggleWemo :: toggleWemo(string URL, int binaryState)
{
    string readBuffer; //buffer to store response from wemo smart switch
    CURL* hnd = curl_easy_init(); //initialize curl handdle
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST"); //set method type to post
	curl_easy_setopt(hnd, CURLOPT_URL, URL.c_str()); //pass url

	struct curl_slist* headers = NULL; //initialize headers
	headers = curl_slist_append(headers, "SOAPACTION: \"urn:Belkin:service:basicevent:1#SetBinaryState\"");//append headers-soap action
	headers = curl_slist_append(headers, "Content-Type: text/xml;");//append headers - content type
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);//pass headers
    //soap body
	string body ="<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                 <s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\
                 <s:Body>\
                 <u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\">\
                 <BinaryState>" + to_string(binaryState) + "</BinaryState>\
                 </u:SetBinaryState>\
                 </s:Body>\
                 </s:Envelope>";
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, body.c_str());//pass body
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);//call writecallback function to write response data
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);//copy response data into string
	curl_easy_perform(hnd);//send request
	//cout<<readBuffer; //debugging 
}


