//Header file for SmartScanner
#include<map>

using namespace std;

class SmartScanner
{
    private:
    std::map <int,std::string> ipAddresses; // private instance variable to store IP addresses of smart devices in network

    
    public:
    int scanLAN(); // function to search network for smart devices
    map <int,std::string> getipAddresses(); //getter for private instace variable ipAddresses
    


};
