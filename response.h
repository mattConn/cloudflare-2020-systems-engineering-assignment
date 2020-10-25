#pragma once

#include <map>
#include <string>
#include <time.h>

using namespace std;

// storing and handling response string
struct Response
{
	// buffer for response
	const static int bufferSize = 1024;
	char buffer[bufferSize] = {0};
	string status;				 // status line
	bool success = true;		  // depends on status
	map<string, string> headers; // header lines
	string body;				 // body of request
	int bytesRead = 0;				// bytes read from response
	time_t time = 0;					// response time

	// constructors
	Response(){};		// default
	Response(char *r); // with response str

	bool setHeadersAndBody(char responseBuffer[]); // get data from response str
};
