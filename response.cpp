#include "response.h"
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

// transform response string
Response::Response(char *responseBuffer)
{
	setHeadersAndBody(responseBuffer);
}

bool Response::setHeadersAndBody(char responseBuffer[])
{
	// get data from response
	int splitPos = string(responseBuffer).find("\r\n\r\n");
	istringstream ssHeaders(string(responseBuffer).substr(0, splitPos)); // headers string
	strcpy(responseBuffer, string(responseBuffer).substr(splitPos+4).c_str()); // body string, mutate buffer

	body = responseBuffer; // store body

	// remove newline and carriage return
	body.pop_back();
	body.pop_back();

	string line; // for getline

	// status line
	if (getline(ssHeaders, line))
	{
		line.pop_back(); // carriage return
		status = line;
		if(status.find("OK") == string::npos) // get error
			success = false;
	}

	// headers
	while (getline(ssHeaders, line) && line != "\r")
	{
		line.pop_back(); // remove carriage return
		int pos = line.find(": ");
		if (pos == string::npos)
		{
			cerr << "Bad header: " << line << endl;
			return false;
		}

		headers.insert({line.substr(0, pos), line.substr(pos + 2)});
	}

	return true;
}
