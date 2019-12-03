// main.cpp - contains main function and user interface

// Header Files
#include "SmartScanner.h" //Scans network for smart devices
#include "DeviceXML.h" //downloads device XML and stores into string
#include "DeviceDescriptionAndServices.h" //parses XML file and displays device desciption and services
#include "ToggleWemo.h" // turns on/off wemo smart plug
#include "GenerateURL.h" // gnerates necessary url to toggle wemo
#include "WemoStatus.h" //gets the status of wemo smart plug
#include <iostream> //to print to console and to get user input

using namespace std;

//main function
int main()
{
    SmartScanner smartScanner; //create SmartScanner object
    DeviceXML deviceXML; //create DeviceXML object
    DeviceDescriptionAndServices deviceDescriptionAndServices; //create DeviceDescriptionAndServices object
    ToggleWemo toggleWemo; //create ToggleWemo object
    GenerateURL generateURL; //create GenerateURL object
    WemoStatus wemoStatus; //Create WemoStatus Object

    
    //User Interface
    bool flag; // Flag for outermost while loop - main menu
    flag = true; // initialize flag
    while(flag)
    {
        int userInput; // variable to store userinput for main menu
        //Display main menu
        cout<<"\t******Welcome to Smart Dashboard******\n";
        cout<<"\t1. Display available smart devices in network\n";
        cout<<"\t2. Exit\n";
        cout<<"\t Please enter input: ";
        cin>>userInput; //get user input for main menu
        if (cin.fail()) // check if user entered an integer or not
        {
            cout<<"\t Please enter a number";
            break;
        }
        
        while((userInput!=1)&&(userInput!=2)) //if user input is wrong prompt again
        {
            cout<<"\tPlease enter correct input: ";
            cin>>userInput;
        }

        if(userInput==2) // if user option is 2, then exit main menu
        {
            flag = false;
        }

        while(userInput == 1) // if useroption is 2, enter sub menu and start scanning network
        {
            int selectedDevice; // variable to store selected device number by user, also serves as variable for submenu
            cout<<"\tScanning network.....\n\n";
            smartScanner.scanLAN(); //invoke scanLan function in SmartScanner class to display avaliable smart devices
            map<int,string> tempipAddressesMap; //temporary map to store device number and ip addresses
            tempipAddressesMap = smartScanner.getipAddresses(); //returns a map with device number and ip

            
            if(tempipAddressesMap.empty()) //if map is empty, ask user to scan again
            {
                cout<<"\tNo devices found, please try again\n";
                break;
            }

            for(auto mapIterator = tempipAddressesMap.cbegin(); mapIterator!=tempipAddressesMap.cend(); ++mapIterator)//loop over map
            {
                cout<<"\tDevice Number: "<<mapIterator->first<<"\n";
                deviceXML.generateXML(mapIterator->second); //download xml using ip address
                string deviceTempXml; 
                deviceTempXml = deviceXML.getXML(); //copy downloaded xml into temporary string
                deviceDescriptionAndServices.displayDeviceInformation(deviceTempXml);//display device information
                cout<<"\n";
            }

            cout<<"\tPlease select a device number or '0' to go back: "; //prompt user to select a device
            cin>>selectedDevice;
            if(cin.fail()) //check if input is integer
            {
                cout<<"\t Please enter a number\n";
                break;
            }
            while ((selectedDevice!=0)&&selectedDevice>tempipAddressesMap.size()) //check if integer is out of range
            {
                cout<<"\t Please enter correct input number: ";
                cin>>selectedDevice;
            }
            if(selectedDevice==0) //if choice is 0 exit sub menu
            {
                break;
            }
                  while(1)//services menu
                  {
                      int servicesMenu; //variable to store services menu choice
                      //int userSelectedDevice;
                      auto iterator = tempipAddressesMap.find(selectedDevice); //find selected device from map
                      deviceXML.generateXML(iterator->second); //generate xml
                      string deviceTempXML; 
                      deviceTempXML = deviceXML.getXML(); //store xml in string
                      deviceDescriptionAndServices.loadDeviceServices(deviceTempXML); // display services
                      map <int,string> tempServicesMap; //temporary services map
                      tempServicesMap = deviceDescriptionAndServices.getDeviceServicesMap();//copy services map to temp services map
                      if(tempServicesMap.empty()) //check if there are no services
                      {
                          cout<<"\t No services found\n";
                          break;
                      }
                      cout<<"\t#"<<"\t"<<"Service\n";  
                      for(auto iterator2 = tempServicesMap.cbegin(); iterator2!=tempServicesMap.cend(); ++iterator2) //display services
                      {
                        cout<<"\t"<<iterator2->first<<"\t"<<iterator2->second<<"\n";
                      }

                      cout<<"\t Please select a service number or '0' to go back: \n"; //prompt user for a service
                      cin>>servicesMenu;
                      if(cin.fail())//check if user option is an integer
                      {
                          cout<<"\tPlease enter a number\n";
                          break;
                      }
                      if(servicesMenu==0) //if user option is 0, go back to previous menu
                      {
                          break;
                      }

                      while((servicesMenu!=0)&&servicesMenu>tempServicesMap.size()) //check if useroption is in range
                      {
                          cout<<"\tPlease enter a valid service number: \n";
                          cin>>servicesMenu;
                      }
                    auto iterator3 = tempServicesMap.find(servicesMenu);//get iterator for selected service
                    if(iterator3->second=="urn:Belkin:service:basicevent:1") //check if selected service is a wemo smart plug basic service
                      {
                          while(1) //enter into final menu
                          {
                                int wemoChoice; //variable to get input from user, toggles wemo
                                cout<<"\t1. Get Device Status\n";
                                cout<<"\t2. Turn on Wemo Smart Plug\n";
                                cout<<"\t3. Turn off Wemo Smart Plug\n";
                                cout<<"\t Please select a service or '0' to go back: \n"; // prompt user
                                cin>>wemoChoice;
                                if(cin.fail()) //check if input is an integer
                                {
                                    cout<<"\tPlease enter a number\n";
                                    break;
                                }
                                while((wemoChoice!=0)&&(wemoChoice>4)&&(wemoChoice<-1))//check if input is in range
                                {
                                    cout<<"\t Please enter a valid number: \n";
                                    cin>>wemoChoice;
                                }
                                if(wemoChoice==0) //if user choice is 0, go baack to previous menu
                                {
                                    break;
                                }
                                
                                if(wemoChoice==1||wemoChoice==2||wemoChoice==3)
                                {
                                   auto iterator3 =  tempipAddressesMap.find(iterator->first);//get index number of map
                                  string temp ;
                                  temp = iterator3->second; // store ip address in a temporary string
                                  if(wemoChoice == 1) // get status of wemo
                                {
                                    wemoStatus.getWemoStatus(generateURL.getWemoToggleURL(temp));
                                }
                                  if(wemoChoice==2) // turn on wemo
                                  {
                                      cout<<"\tTurning ON Wemo Smart Plug....\n";
                                      toggleWemo.toggleWemo(generateURL.getWemoToggleURL(temp),1);//toggle wemo
                                      wemoStatus.getWemoStatus(generateURL.getWemoToggleURL(temp)); //show status
                                  }
                                  if(wemoChoice==3) //turn off wemo
                                  {
                                      cout<<"\tTurning OFF Wemo Smart Plug....\n";
                                      toggleWemo.toggleWemo(generateURL.getWemoToggleURL(temp),0);//toggle wemo
                                      wemoStatus.getWemoStatus(generateURL.getWemoToggleURL(temp)); //shhow status
                                  }
                                }


                          }
                      }
                    else
                    {
                        cout<<"\t Service implementation not found";
                    }
                    

                   
                    
                      

                  }
            
            

        }
    }
    
    


return -1;
}
