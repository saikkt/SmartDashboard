//DeviceDescriptionAndServices Header file
#include <string>
#include <map>
using namespace std;
class DeviceDescriptionAndServices
{
    private:
        map<int,string> deviceServicesMap; //private instance variable to store services in a map
    public:
        void displayDeviceInformation(string XML); //function to display device information
        void loadDeviceServices(string XML); //function to load services into map
        map <int,string> getDeviceServicesMap(); //function to return services map

};