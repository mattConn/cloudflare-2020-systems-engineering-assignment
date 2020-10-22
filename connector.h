#include <iostream>
#include <string>

using namespace std;

class Connector {
 	// socket file descriptor
	int fileDescriptor = 0;

	int port = 80;

	char *hostIPString;

	// buffer for response
	char responseString[1024] = {0};

	// connection status
	bool isConnected = false;

	// host information struct
	struct hostent *hostEntry;

	// host address struct
	struct sockaddr_in *hostAddress;

public:

	Connector(); // default constructor

	// methods
	//========

	// port accessors
	bool setPort(int portNum);
	int getPort() { return port; };

	string getHostIPString(){ return hostIPString; };

	bool connectTo(string &urlString); // connect to url 
	bool makeHTTPRequest(); // make request after connection 
	string getHTTPResponse(); // get response string
};
