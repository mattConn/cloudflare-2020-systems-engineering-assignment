#include <iostream>
#include <string>
#include "connector.h" 

using namespace std;

int main()
{
	string url = "foobiebletch.net";
	Connector c;
	c.connectTo(url);
	
	if(!c.isConnected())
		cout << "Could not connect to host " << url << endl;
	else
	{
		cout << "Connected to host " << url << endl;
		cout << "Host IP: " << c.getHostIPString() << endl;
	}
	


	return 0;
}
