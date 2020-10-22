#include <iostream>
#include <netdb.h>
#include <string>

using namespace std;

class ClientSocket {
 	// socket file descriptor
	int fileDescriptor = 0;

	int hostPort;

	char *hostIPString;

	// buffer for response
	char responseString[1024] = {0};

	// connection status
	bool connected = false;

	// host information struct
	struct hostent *hostEntry;

	// host address struct
	struct sockaddr_in hostAddress;

public:

	ClientSocket(); // default constructor
	ClientSocket(string &url); // constructor with url 

	// methods
	//========

	// set file descriptor
	bool makeSocket(); 

	// convert url to IPv4 IP address string
	bool URLToIP(string &url);

	// port accessors
	bool setHostPort(int port);
	int getHostPort() { return hostPort; };

	string getHostIPString(){ return hostIPString; };

	bool isConnected(){ return connected; };

	bool connectTo(string &urlString); // connect to url 
	bool makeHTTPRequest(); // make request after connection 
	string getHTTPResponse(); // get response string
};
