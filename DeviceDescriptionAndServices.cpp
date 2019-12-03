//Implementation of DeviceDescritionAndServices
#include "DeviceDescriptionAndServices.h"
#include "tinyxml2.h"
#include <iostream>

void DeviceDescriptionAndServices :: displayDeviceInformation (string XML) //displays device information
{
    tinyxml2::XMLDocument xml_doc;//initialize xml document
    tinyxml2::XMLError eResult = xml_doc.Parse(XML.c_str());//parse document
    if (eResult != tinyxml2::XML_SUCCESS) //check if parsing is success
    {
        cout<<"Error in parsing XML\n";
        return;
    }
    tinyxml2::XMLElement* deviceDetails = xml_doc.FirstChildElement( "root" )->FirstChildElement("device");//traverse up to device
   // ******Device Details******
    std::cout<<"\tManufacturer: "<<deviceDetails->FirstChildElement("manufacturer")->GetText()<<"\n"; //get manufacturer
    std::cout<<"\tModel: "<<deviceDetails->FirstChildElement("modelName")->GetText()<<"\n";    //get model name
    std::cout<<"\tFriendly Name: "<<deviceDetails->FirstChildElement("friendlyName")->GetText()<<"\n";//get friendly name
}

void DeviceDescriptionAndServices :: loadDeviceServices(string XML) //loads services into map
{
    int indexNumber = 1; //variable to store service number
	tinyxml2::XMLDocument xml_doc; //initialize xxml document
   	tinyxml2::XMLError eResult = xml_doc.Parse(XML.c_str()); //parse xml document
	if (eResult != tinyxml2::XML_SUCCESS) //check if parsing was correct
	{
		cout<<"Parsing XML error\n";
        return;
	}
	tinyxml2::XMLElement* child2 = xml_doc.FirstChildElement( "root" )->FirstChildElement("device")->FirstChildElement("serviceList");//traverse upto service list
	for (tinyxml2::XMLElement* child = child2->FirstChildElement(); child != NULL; child = child->NextSiblingElement())//loop through services
	{
	 this->deviceServicesMap.insert(pair<int,string>(indexNumber,child->FirstChildElement("serviceType")->GetText()));//insert services and index number into map
	 indexNumber++;
	}
}

map<int, string> DeviceDescriptionAndServices :: getDeviceServicesMap()//function returns services map
{
    return this->deviceServicesMap;
}