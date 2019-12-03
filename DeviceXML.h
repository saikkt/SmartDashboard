//DeviceXML header file
#include <string> 
using namespace std;
class DeviceXML
{
private:
    string XML; //private instance variable to store XML in string format
public:
    void generateXML(string url); //generates XML by sending http request
    string getXML(); //function to get XML in string format
};

