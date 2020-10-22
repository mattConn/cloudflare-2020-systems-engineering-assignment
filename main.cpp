#include <iostream>
#include <string>
#include "connector.h" 

using namespace std;

int main()
{
	string url = "foobiebletch.net";
	Connector c;
	c.connectTo(url);
	cout << c.getHostIPString() << endl;

	return 0;
}
