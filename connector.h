#include <iostream>
#include <string>

using namespace std;

class Connector {
 	// socket file descriptor
	int fileDescriptor = 0;

	int port = 80;

	// buffer for response
	char responseString[1024] = {0};

	// connection status
	bool isConnected = false;

	// host information struct
	struct hostent *hostEntry;

	// host address
	struct sockaddr_in *serverAddress;

public:

	Connector(); // default constructor

	// methods
	//========

	// port accessors
	inline bool setPort(int portNum){
		if(portNum<0) return false;

		return (port = portNum);
	};

	inline int getPort() { return port; };

	bool connectTo(string &url); // connect to url 
	bool makeHTTPRequest(); // make request after connection 
	string getHTTPResponse(); // get response string
};
