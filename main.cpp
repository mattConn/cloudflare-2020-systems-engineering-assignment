#include <iostream>
#include <string>
#include "clientsocket.h" 

using namespace std;

void printHelp();

int main(int argc, char *argv[])
{
	string url;
	int requestCount = 1;

	// handle cli args
	//================

	// missing args
	if(argc < 2)
	{
		printHelp();
		return 1;
	}

	// check args
	for(int i=1; i < argc; i++)
	{
		string arg = argv[i];

		// help
		if(arg == "--help")
		{
			printHelp();
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
		cout << "Could not connect to host " << url << endl;
	else
	{
		cout << "Connected to host " << url << endl;
		cout << "Host IP: " << socket.getHostIP() << endl;
	}

	socket.makeRequest();
	cout << "\tResponse:" << endl;
	cout << socket.getResponse() << endl;

	return 0;
}

void printHelp()
{
	cout << "Help String" << endl;
}
