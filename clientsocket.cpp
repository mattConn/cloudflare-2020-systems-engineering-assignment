#include "clientsocket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string.h>

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
	// set fd
	if (!makeSocket())
	{
		cerr << "Could not make socket. Exiting." << endl;
		exit(EXIT_FAILURE);
	}

	// get ip from url, convert to IPv4 address string
	if (!setHostIP(url))
	{
		cerr << "Bad address:" << url << "." << endl;
		exit(EXIT_FAILURE);
	}

	// connect
	if (!connectToHost())
	{
		cerr << "Could not connect to host at " << hostIP << endl;
		exit(EXIT_FAILURE);
	}

	// prepare request
	setRequest(hostURI, hostDomain);
}

void ClientSocket::formatHostURL(string url)
{
	// strip protocol
	//===============

	// only if specified
	int pos = url.find("//");
	if (pos != string::npos)
		url.erase(0, pos + 2);

	// split url into domain and uri
	// =============================
	pos = url.find("/");
	if (pos != string::npos)
	{
		hostDomain = url.substr(0, pos);
		hostURI = url.substr(pos);
	}
	else
	{
		// no uri
		hostDomain = url;
		hostURI = "/";
	}
}

bool ClientSocket::makeSocket()
{
	// create socket using IPv4, TCP, IP
	fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (fileDescriptor < 0)
		return false;

	return true;
}

// convert url
bool ClientSocket::setHostIP(string &url)
{

	// split url into domain and uri
	formatHostURL(url);

	// get host data, for converting url to ip
	hostEntry = gethostbyname(hostDomain.c_str());

	// convert ip to IPv4 dotted decimal str
	hostIP = inet_ntoa(*((struct in_addr *)hostEntry->h_addr));

	// convert address from string to bin, store in sin_addr
	if (inet_pton(AF_INET, hostIP, &hostAddress.sin_addr) <= 0)
		return false;

	hostURL = url;

	return true;
}

// set port
bool ClientSocket::setHostPort(int port)
{
	if (port < 0)
		return false;

	hostPort = port;

	// set host port with network endianness
	hostAddress.sin_port = htons(hostPort);
	return true;
}

bool ClientSocket::connectToHost()
{
	// connect to address and store sock fd
	if (connect(fileDescriptor, (struct sockaddr *)&hostAddress, sizeof(hostAddress)) < 0)
		return false;
	connected = true;

	return true;
}

// make request to host after connection
bool ClientSocket::readRequest()
{
	if(send(fileDescriptor, (char *)request.c_str(), strlen(request.c_str()), 0) < 0)
		return false;
	else
		return true;
}

bool ClientSocket::readResponse()
{
	clock_t timeBegin = clock(); // time read
	int currentBytesRead = read(fileDescriptor, response.buffer, response.bufferSize);
	clock_t timeEnd = clock();

	if (currentBytesRead == 0) // host closed connection
		return false;

	// check if headers were stored
	if(response.headers.empty()) setHeaders();

	response.body += response.buffer; // append body

	response.time += ((float)(timeEnd - timeBegin))/CLOCKS_PER_SEC; // add to response time
	response.size += currentBytesRead; // add to byte count

	// if chunked
	if (response.headers["Transfer-Encoding"] == "chunked")
	{
		// read until zero chunk size
		if(response.body.find("\r\n0\r\n") == string::npos)
		{
			response.numChunks++;
			readResponse();
		}else{
			response.numChunks++; // count last or only chunk

			// trim from zero
			response.body = response.body.substr(0,response.body.find("\r\n0\r\n"));
			response.body += "\r\n";
		}
	}

	return true;
}

void ClientSocket::setRequest(string uri, string domain)
{
	request = "GET " + uri + " HTTP/1.1\r\nHost: " + domain + "\r\n\r\n";
};

bool ClientSocket::setHeaders()
{
	int splitPos = string(response.buffer).find("\r\n\r\n");
	istringstream ssHeaders(string(response.buffer).substr(0, splitPos));		   // headers string
	strcpy(response.buffer, string(response.buffer).substr(splitPos + 4).c_str()); // body string, mutate buffer

	string line; // for getline

	// status line
	if (getline(ssHeaders, line))
	{
		line.pop_back(); // carriage return
		response.statusLine = line.substr(line.find(" ")+1);

		// get status code 
		response.statusCode = stoi(response.statusLine.substr(0,3));

		// set flag
		if (response.statusCode >= 200 && response.statusCode < 300)
			response.wasSuccessful = true;
	}

	// headers into map
	while (getline(ssHeaders, line) && line != "\r")
	{
		line.pop_back(); // remove carriage return
		int pos = line.find(": ");
		if (pos == string::npos)
		{
			cerr << "Bad header: " << line << endl;
			return false;
		}

		response.headers.insert({line.substr(0, pos), line.substr(pos + 2)});
	}

	return true;
}