#include "connector.h"
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <iostream> 
#include <string> 

using namespace std;

// constructor
Connector::Connector()
{
	// create socket using IPv4, TCP, IP; exit on failure
	fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(fileDescriptor < 0) 
	{
		cerr << "Could not create socket." << endl;
		exit(EXIT_FAILURE);
	}

	hostAddress->sin_family = AF_INET; // set to IPv4
}

// set port
bool Connector::setPort(int portNum){
	if(portNum<0) return false;

	port = portNum;

	// set host port with network endianness
	hostAddress->sin_port = htons(port); 
	return true;
}

// connect to url
bool Connector::connectTo(string &urlString)
{

	hostEntry = gethostbyname(urlString.c_str()); // get host data, for converting url to ip
	hostIPString = inet_ntoa(*( (struct in_addr*) hostEntry->h_addr)); // convert ip to IPv4 dotted decimal str

	return true;
}

// make request to url after connection
bool Connector::makeHTTPRequest()
{
	if(!isConnected) return false;

	return true;
}
