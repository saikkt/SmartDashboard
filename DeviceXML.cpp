#include "DeviceXML.h"
#include <curl/curl.h>



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) //function to write returned data into string
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void DeviceXML:: generateXML(string url) //function to send http request and get devices XML
{
    string readBuffer;
    CURL *hnd = curl_easy_init(); //intialize curl easy handle
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");//set method type
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str()); //pass in url
    struct curl_slist *headers = NULL; //initialize headers
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers); //pass headers
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);//call write function
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);//write data into string
    CURLcode ret = curl_easy_perform(hnd); //send request
    this->XML = readBuffer; //set XML

}

string DeviceXML :: getXML() //returns device XML in string format
{
    return this->XML;
}