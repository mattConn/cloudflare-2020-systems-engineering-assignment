#include "response.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// transform response string
Response::Response(string r)
{
	parse(r);
}

bool Response::parse(string r)
{
	// get data from response
	istringstream ss(r);
	string line;

	// status line
	if(getline(ss, line)) status = line;

	// headers
	while(getline(ss, line) && line != "\r")
	{
		line.pop_back(); // remove carriage return
		int pos = line.find(": ");
		if(pos == string::npos)
		{
			cerr << "Bad header: " << line << endl;
			return false;
		}

		headers.insert({line.substr(0,pos), line.substr(pos+2)});
	}

	return true;
}
