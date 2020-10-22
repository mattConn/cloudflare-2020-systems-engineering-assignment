#include <iostream>
#include <string>
#include "clientsocket.h" 

using namespace std;

void printHelp();

int main(int argc, char *argv[])
{
	string url;

	// handle cli args
	//================

	// missing args
	if(argc < 2)
	{
		printHelp();
		return 1;
	}

	// check args
	for(int i=0; i < argc; i++)
	{
		string arg = argv[i];

		// help
		if(arg == "--help")
		{
			printHelp();
			return 0;
		}

		// url 
		if(arg == "--url")
		{
			if(i+1 >= argc)
			{
				cerr << "Missing URL." << endl;
				return 0;
			}
		}
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
