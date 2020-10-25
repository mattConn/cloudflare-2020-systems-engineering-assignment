#include <iostream>
#include <netdb.h>
#include <string>
#include "response.h"

struct response;

using namespace std;

class ClientSocket {
 	// socket file descriptor
	int fileDescriptor = 0;

	// host identifiers
	int hostPort;
	char *hostIP;
	string hostURL;
	string hostURI;
	string hostDomain;


	// buffer for response
	// char responseBuffer[1024] = {0};
	// int bytesRead = 0;
	// int totalBytesRead = 0;

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

	// for storing response data
	Response response;

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

	bool readResponse(); // make request after connection 

	// request string accessors
	string getRequest(){ return request; };
	void setRequest(string uri, string domain);

};
