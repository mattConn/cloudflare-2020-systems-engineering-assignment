#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "clientsocket.h"

using namespace std;

void printMsg(string msg);

int main(int argc, char *argv[])
{
	// for help display
	string helpString = "Usage: makereq --url <url> [--profile <number of requests>]";

	int responseCount = 0;
	int requestCount = 0;
	int requestAttempts = 0;

	// set via args
	int requestQuota = 1; // how many requests we want to make
	string url;

	// handle args
	// ===========
	if (argc < 2)
	{
		cout << helpString << endl;
		return 1;
	}

	for (int i = 1; i < argc; i++)
	{
		char flag = argv[i][2];

		switch (flag)
		{
		case 'h': // help
			cout << helpString << endl;
			return 0;

		case 'u': // url
			url = argv[++i];
			break;

		case 'p': // profiling
			requestQuota = stoi(argv[++i]);
			break;

		default:
			cout << helpString << endl;
			return 1;
		}
	}

	if (url.empty())
	{
		cout << helpString << endl;
		return 1;
	}

	// ===============
	// end handle args

	// profiling variables
	// ===================

	// response times
	vector<float> responseTimes;
	int successCount = 0; // count successful responses

	// repsonse sizes in bytes
	vector<int> responseSizes;

	vector<string> errorStatuses;

	// make requests
	// =============

	while (requestAttempts < requestQuota)
	{
		ClientSocket socket(url); // make socket fd, convert url and connect

		if (socket.sendRequest()) // successful send
		{
			requestCount++;

			if (socket.readResponse()) // successful read
			{
				responseCount++;

				// display body
				cout << endl;
				printMsg("Body of response " + to_string(responseCount) + "/" + to_string(requestQuota) + ":");
				cout << endl;

				cout << socket.response.body << endl;

				// record time
				responseTimes.push_back(socket.response.time);
				responseSizes.push_back(socket.response.size);

				// count successes, store unique errors
				if(socket.response.wasSuccessful)
					successCount++;
				else
				{
					if(find(errorStatuses.begin(), errorStatuses.end(), socket.response.statusLine) == errorStatuses.end() )
						errorStatuses.push_back(socket.response.statusLine);
				}
			} // end response read
		} // end request send

		requestAttempts++;
	}
	// end making requests

	// sort times, sum
	sort(responseTimes.begin(), responseTimes.end());
	float sumTimes = 0;
	for (auto &t : responseTimes)
		sumTimes += t;

	// sort sizes
	sort(responseSizes.begin(), responseSizes.end());

	// calc success rate
	int successRate = (((float)successCount) / ((float)responseCount)) * 100;

	// display profiling data
	cout << endl;
	printMsg(url);
	printMsg("Requests made: " + to_string(requestCount));
	printMsg("Fastest response time: " + to_string(responseTimes.front()) + "s");
	printMsg("Slowest response time: " + to_string(responseTimes.back()) + "s");
	printMsg("Mean response time: " + to_string(sumTimes / responseTimes.size()) + "s");
	printMsg("Median response time: " + to_string(responseTimes[responseTimes.size() / 2]) + "s");
	printMsg("Success rate: " + to_string(successRate) + "%");
	printMsg("Error codes:");
	for(auto &err : errorStatuses) cout << err << endl;
	printMsg("Smallest response: " + to_string(responseSizes.front()) + " bytes");
	printMsg("Largest response: " + to_string(responseSizes.back()) + " bytes");

	return 0;
}

void printMsg(string msg)
{
	cout << "[ " + msg + " ]" << endl;
}