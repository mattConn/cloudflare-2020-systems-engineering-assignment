#include "response.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// transform response string
Response::Response(string rawResponse)
{
	setHeadersAndBody(rawResponse);
}

bool Response::setHeadersAndBody(string rawResponse)
{
	// get data from response
	int splitPos = rawResponse.find("\r\n\r\n");
	istringstream ssHeaders(rawResponse.substr(0, splitPos));
	body = rawResponse.substr(splitPos+4);
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
