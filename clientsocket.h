#include <iostream>
#include <netdb.h>
#include <string>

using namespace std;

class ClientSocket {
 	// socket file descriptor
	int fileDescriptor = 0;

	int hostPort;

	// host identifiers
	char *hostIP;
	string hostURL;
	string hostURI;
	string hostDomain;

	// buffer for response
	char response[1024] = {0};
	string request;

	// connection status
	bool connected = false;

	// host information struct
	struct hostent *hostEntry;

	// host address struct
	struct sockaddr_in hostAddress;

	// helper function for splitting url
	void formatHostURL(string url);

public:

	ClientSocket(); // default constructor
	ClientSocket(string &url); // constructor with url 

	// methods
	//========

	// set file descriptor
	bool makeSocket(); 

	// convert url to IPv4 IP address string
	bool setHostIP(string &url);

	// port accessors
	bool setHostPort(int port);
	int getHostPort() { return hostPort; };

	string getHostIP(){ return hostIP; };
	string getHostURL(){ return hostURL; };

	bool isConnected(){ return connected; };

	bool connectToHost(); 
	bool makeRequest(); // make request after connection 
	string getResponse(){ return response; }; // get response string
};
