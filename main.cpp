#include <iostream>
#include <string>
#include "clientsocket.h"

using namespace std;

int main(int argc, char* argv[])
{
	// for help display
	string helpString = "Usage: makereq --url <url> [--profile <number of requests>]";

	int numRequests = 1;
	string url;

	// handle args
	if(argc < 2)
	{
		cout << helpString << endl;
		return 1;
	}

	for(int i=1; i < argc; i++)
	{
		char flag = argv[i][2];

		switch(flag)
		{
			case 'h': // help
				cout << helpString << endl;
				return 0;

			case 'u': // url
				url = argv[++i];
			break;

			case 'p':
				numRequests = stoi(argv[++i]);
			break;

			default:
				cout << helpString << endl;
				return 1;
		}
	}

	if(url.empty())
	{
		cout << helpString << endl;
		return 1;
	}

	while(numRequests > 0)
	{
		ClientSocket socket(url);
		socket.makeRequest();
		socket.readResponse();
		cout << socket.response.body << endl;
		
		numRequests--;
	}
	

	return 0;
}
