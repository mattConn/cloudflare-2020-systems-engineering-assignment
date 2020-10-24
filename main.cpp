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

	// ===================
	// end handle cli args


	// make socket, parse/convert url and connect
	// ==========================================

	ClientSocket socket(url);
	
	if(!socket.isConnected()) // check connection
	{
		cout << "Could not connect to host " << url << endl;
		return 1;
	}
	
	// ready to make requests
	// ======================
	
	// display request
	cout << "\t\t\t<<< REQUEST >>>" << endl;
	cout << socket.getRequest();

	// make response obj for holding headers, body
	Response response;

	// initial request
	socket.makeRequest();
	socket.readResponse();
	response.body += socket.getRawResponse();
	response.body.pop_back(); // chomp
	response.body.pop_back(); // and again

	// read headers
	response.setHeaders(socket.getRawResponse());

	// read on single send if chunked
	if(response.headers["Transfer-Encoding"] == "chunked")
	{
	
		while(socket.readResponse())
		{

			response.body += socket.getRawResponse();
			response.body.pop_back(); // chomp
			response.body.pop_back(); // and again

			// look for 0 chunk size and break
			if(socket.getRawResponse().find("\r\n0\r\n") != string::npos) break;

		}
	}

	cout << "\t\t\t<<< RESPONSE >>>" << endl;
	cout << response.body << endl;

	return 0;
}
