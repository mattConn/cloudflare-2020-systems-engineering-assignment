#include "connector.h"
#include <sys/socket.h> 
#include <netdb.h> 
#include <iostream> 
#include <string> 

using namespace std;

// constructor
Connector::Connector()
{
	// create socket using IPV4, TCP, IP; exit on failure
	fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(fileDescriptor < 0) 
	{
		cerr << "Could not create socket." << endl;
		exit(EXIT_FAILURE);
	}
}

// connect to url
bool Connector::connectTo(string &url)
{
	return true;
}

// make request to url after connection
bool Connector::makeHTTPRequest()
{
	if(!isConnected) return false;

	return true;
}
