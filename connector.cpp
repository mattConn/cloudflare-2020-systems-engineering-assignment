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

	hostAddress.sin_family = AF_INET; // set to IPv4
	setHostPort(80);
}

// set port
bool Connector::setHostPort(int port){
	if(port<0) return false;

	hostPort = port;

	// set host port with network endianness
	hostAddress.sin_port = htons(hostPort); 
	return true;
}

// connect to url
bool Connector::connectTo(string &urlString)
{

 	// get host data, for converting url to ip
	hostEntry = gethostbyname(urlString.c_str());

	// convert ip to IPv4 dotted decimal str
	hostIPString = inet_ntoa(*( (struct in_addr*) hostEntry->h_addr)); 

	// convert address from string to bin
    if(inet_pton(AF_INET, hostIPString, &hostAddress.sin_addr) <= 0) return false;

	// connect to address and store sock fd
    if (connect(fileDescriptor, (struct sockaddr *)&hostAddress, sizeof(hostAddress)) < 0) return false;
	connected = true;


	return true;
}

// make request to url after connection
bool Connector::makeHTTPRequest()
{

	return true;
}
