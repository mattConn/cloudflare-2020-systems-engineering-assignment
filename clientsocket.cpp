#include "clientsocket.h"
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <iostream> 
#include <string> 

using namespace std;

// constructor
ClientSocket::ClientSocket()
{
	hostAddress.sin_family = AF_INET; // set to IPv4
	setHostPort(80);
}

// constructor with url
ClientSocket::ClientSocket(string &url) : ClientSocket()
{
	if(!makeSocket())
	{
		cerr << "Could not make socket. Exiting." << endl;
		exit(EXIT_FAILURE);
	}
	
}

bool ClientSocket::makeSocket()
{
	// create socket using IPv4, TCP, IP
	fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(fileDescriptor < 0) return false;

	return true;
}

// convert url
bool ClientSocket::URLToIP(string &url)
{
 	// get host data, for converting url to ip
	hostEntry = gethostbyname(url.c_str());

	// convert ip to IPv4 dotted decimal str
	hostIPString = inet_ntoa(*( (struct in_addr*) hostEntry->h_addr)); 

	// convert address from string to bin, store in sin_addr
    if(inet_pton(AF_INET, hostIPString, &hostAddress.sin_addr) <= 0) return false;

	return true;
}

// set port
bool ClientSocket::setHostPort(int port){
	if(port<0) return false;

	hostPort = port;

	// set host port with network endianness
	hostAddress.sin_port = htons(hostPort); 
	return true;
}

// connect to url
bool ClientSocket::connectToHost()
{
	// connect to address and store sock fd
    if (connect(fileDescriptor, (struct sockaddr *)&hostAddress, sizeof(hostAddress)) < 0) return false;
	connected = true;


	return true;
}

// make request to url after connection
bool ClientSocket::makeHTTPRequest()
{

	return true;
}
