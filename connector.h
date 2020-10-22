#include <iostream>
#include <string>

using namespace std;

class Connector {
 	// socket file descriptor
	int fileDescriptor = 0;

	// buffer for response
	char responseString[1024] = {0};

	// connection status
	bool connected = false;

public:

	Connector(); // default constructor

	// methods
	//========

	bool makeHTTPRequest(string &url); // make request to specified url 
	string getHTTPResponse(); // get response string
};
