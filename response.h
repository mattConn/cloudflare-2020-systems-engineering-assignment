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
	char buffer[bufferSize] = {0}; // buffer read() writes to
	string status;				 // status line
	bool success = true;		  // depends on status
	map<string, string> headers; // header lines
	string body;				 // body of response
	int bytesRead = 0;				// bytes read from response
	float time = 0;					// response time

	// constructors
	Response(){};
};
