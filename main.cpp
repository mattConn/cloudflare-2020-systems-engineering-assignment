#include <iostream>
#include <string>
#include <string.h>
#include "clientsocket.h" 
#include "response.h" 

using namespace std;

int main(int argc, char *argv[])
{
	string helpString = "Usage: makereq --url <url> [--profile <number of requests>]";
	string url;
	int requestCount = 1;

	// handle cli args
	//================

	// missing args
	if(argc < 2)
	{
		cerr << "Missing args." << endl;
		cout << helpString << endl;
		return 1;
	}

	// check args
	for(int i=1; i < argc; i++)
	{
		string arg = argv[i];

		// help
		if(arg == "--help")
		{
			cout << helpString << endl;
			return 0;
		}

		// url 
		else if(arg == "--url")
		{
			if(i+1 >= argc)
			{
				cerr << "Missing URL." << endl;
				return 1;
			}
			url = argv[++i];
		}

		// profile 
		else if(arg == "--profile")
		{
			if(i+1 >= argc)
			{
				cerr << "Missing request count." << endl;
				return 1;
			}

			// test for stoi failure
			try {
				requestCount = stoi(argv[++i]);
			} catch(exception &err){
				cerr << "Invalid request count " << argv[i] << ". Must be integer." << endl;
				return 1;
			}

			// requestCount must be > 0
			if(requestCount < 1)
			{
				cerr << "Invalid request count " << requestCount << ". Must be integer > 0." << endl;
				return 1;
			}
		}
		// invalid args
		else {
			cerr << "Invalid argument " << arg << ". Try --help" << endl;
			return 1;
		}
	}
	
	// check if url specified
	if(url.size() == 0)
	{
		cerr << "Missing URL. Specify with --url" << endl;
		return 1;
	}


	ClientSocket socket(url);
	/*
	socket.makeSocket();
	socket.setHostIP(url);
	socket.connectToHost();
	*/
	
	if(!socket.isConnected())
	{
		cout << "Could not connect to host " << url << endl;
		return 1;
	}
	
	// ready to make requests
	// ======================

	// terminal text coloring
	const char *greenText = "\033[1;32m";
	const char *defaultText = "\033[0m";

	
	cout << greenText << "[Request]" << defaultText << endl;
	cout << socket.getRequest();

	Response r;

	int responseCount = 1;
	while(responseCount <= requestCount)
	{
		socket.makeRequest();
		cout << greenText << "[Response #" << responseCount << "]" << defaultText << endl;
		cout << socket.getResponse() << endl;
		r.parse(socket.getResponse());

		// check transfer encoding
		if(r.headers["Transfer-Encoding"] == "chunked")
			cout << "CHUNKED" << endl;
		else
			cout << "NOT CHUNKED" << endl;

		responseCount++;
	}

	return 0;
}
