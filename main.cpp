#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "clientsocket.h"
#include "response.h"

using namespace std;

// green terminal text
void printGreen(string msg);

int main(int argc, char *argv[])
{

	// for help display
	string helpString = "Usage: makereq --url <url> [--profile <number of requests>]";
	string url;			  // url buffer
	int requestCount = 1; // number of requests to make

	// handle cli args
	//================

	// missing args
	if (argc < 2)
	{
		cerr << "Missing args." << endl;
		cout << helpString << endl;
		return 1;
	}

	// check args
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];

		// help
		if (arg == "--help")
		{
			cout << helpString << endl;
			return 0;
		}

		// url
		else if (arg == "--url")
		{
			if (i + 1 >= argc)
			{
				cerr << "Missing URL." << endl;
				return 1;
			}
			url = argv[++i];
		}

		// profile
		else if (arg == "--profile")
		{
			if (i + 1 >= argc)
			{
				cerr << "Missing request count." << endl;
				return 1;
			}

			// test for stoi failure
			try
			{
				requestCount = stoi(argv[++i]);
			}
			catch (exception &err)
			{
				cerr << "Invalid request count " << argv[i] << ". Must be integer." << endl;
				return 1;
			}

			// requestCount must be > 0
			if (requestCount < 1)
			{
				cerr << "Invalid request count " << requestCount << ". Must be integer > 0." << endl;
				return 1;
			}
		}
		// invalid args
		else
		{
			cerr << "Invalid argument " << arg << ". Try --help" << endl;
			return 1;
		}
	}

	// check if url specified
	if (url.size() == 0)
	{
		cerr << "Missing URL. Specify with --url" << endl;
		return 1;
	}

	// ===================
	// end handle cli args

	// make socket, parse/convert url, connect then request
	// ====================================================

	int responseCount = 0;
	vector<Response> responseList;
	// response loop
	while (responseCount < requestCount)
	{

		ClientSocket socket(url);

		if (!socket.isConnected()) // check connection
		{
			cout << "Could not connect to host " << url << endl;
			return 1;
		}

		// ready to make requests
		// ======================

		// display request on first iteration
		if(responseCount ==0)
		{
			printGreen("Request");
			cout << socket.getRequest();
			printGreen("Making "+to_string(requestCount)+" Request(s)...");
			cout << endl;
		}

		// initial request
		socket.makeRequest();
		socket.readResponse();

		// read headers,status into response obj map
		Response response(socket.getRawResponse());

		response.body += socket.getRawResponse();
		response.body.pop_back(); // chomp
		response.body.pop_back(); // and again

		// read from stream until 0 char if chunked
		if (response.headers["Transfer-Encoding"] == "chunked")
		{
			while (socket.readResponse())
			{
				response.body += socket.getRawResponse();
				response.body.pop_back(); // chomp
				response.body.pop_back(); // and again

				// look for 0 chunk size and break
				if (socket.getRawResponse().find("\r\n0\r\n") != string::npos)
					break;
			}
		}

		// display response body on first iteration
		if(responseCount == 0)
		{
			printGreen("Response Body");
			cout << response.body << endl;
		}

		responseCount++;

		// store total byte size
		response.bytesRead = socket.getTotalBytesRead();

		responseList.push_back(response); // store response obj

	} // end requests

	// gather profiling data
	// =====================
	if(requestCount > 1)
	{
		// store error codes
		vector<string> errCodes;
		for(auto &r: responseList)
		{
			if(r.status.find("OK") == string::npos)
			{
				// unique error code?
 				if(find(errCodes.begin(), errCodes.end(), r.status) == errCodes.end())
					errCodes.push_back(r.status);
			}
		}

		// display 
		printGreen("Profiling Results");
		cout << endl;
		printGreen("Requests made: "+to_string(responseList.size()));
		if(!errCodes.empty())
		{
			printGreen("Error Codes");
			for(auto &c : errCodes) cout << c << endl;
			cout << endl;
		}
	}


	return 0;
}

void printGreen(string msg)
{
	cout << "\033[1;32m"
		 << "[ " << msg << " ]"
		 << "\033[0m" << endl;
}
