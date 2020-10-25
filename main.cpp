#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "clientsocket.h"

using namespace std;

void printMsg(string msg);

int main(int argc, char* argv[])
{
	// for help display
	string helpString = "Usage: makereq --url <url> [--profile <number of requests>]";

	int requestCount = 0;
	int requestQuota = 1;
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
				requestQuota = stoi(argv[++i]);
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
	// ===============
	// end handle args

	// profiling variables
	// ===================
	vector<float> responseTimes;
	while(requestCount < requestQuota)
	{
		ClientSocket socket(url);
		socket.makeRequest();
		socket.readResponse();
		cout << socket.response.body << endl;

		// record time
		responseTimes.push_back(socket.response.time);
		
		requestCount++;
	}

	// sort times
	sort(responseTimes.begin(), responseTimes.end());
	float sumTimes = 0;
	for(auto &t : responseTimes) sumTimes += t;

	// profiling data
	cout << endl;
	printMsg("Requests made: "+to_string(requestCount));
	printMsg("Fastest response time: "+to_string(responseTimes.front())+"s");
	printMsg("Slowest response time: "+to_string(responseTimes.back())+"s");
	printMsg("Mean response time: "+to_string(sumTimes/responseTimes.size())+"s");
	printMsg("Median response time: "+to_string(responseTimes[responseTimes.size()/2])+"s");


	

	return 0;
}

void printMsg(string msg)
{
	cout << "[ "+msg+" ]" << endl;
}