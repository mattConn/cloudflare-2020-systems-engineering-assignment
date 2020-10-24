#include "response.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// transform response string
Response::Response(string responseBuffer)
{
	setHeadersAndBody(responseBuffer);
}

bool Response::setHeadersAndBody(string responseBuffer)
{
	// get data from response
	int splitPos = responseBuffer.find("\r\n\r\n");
	istringstream ssHeaders(responseBuffer.substr(0, splitPos));
	body = responseBuffer.substr(splitPos+4);
	string line;

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
