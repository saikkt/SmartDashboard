#include "GenerateURL.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <regex>
#include<string>



using namespace std;

string GenerateURL :: getWemoToggleURL(string URL) //function to generate url required to toggle wemo
{
	string s (URL);
	smatch m;
	regex e ("\\http://\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\:\\d{1,5}");
    while(regex_search (s,m,e))
	{
		for(auto url:m)
		return (string) url + "/upnp/control/basicevent1";
		//cout<<url<<" ";
		//cout<<endl;
		//s = m.suffix().str();
	}

}