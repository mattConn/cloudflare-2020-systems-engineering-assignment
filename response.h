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
	string statusLine;				   // status line
	int statusCode;
	bool wasSuccessful = false;

	map<string, string> headers; // header lines
	int numChunks = 0;			 // number of chunks for chunked response
	string body;				 // body of response
	int size = 0;				 // bytes read from response
	float time = 0;				 // response time

	// constructors
	Response(){};
};
