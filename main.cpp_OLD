#include <iostream>
#include <string>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "clientsocket.h"
#include "response.h"

using namespace std;

// green terminal text
void printMsg(string msg);

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
	vector<Response> responseList; // all responses
	vector<int> responseTimes; // all response times

	// timing
	time_t timeBegin;
	time_t timeEnd;
	time_t timeSum = 0; // for mean time
	// response loop
	while (responseCount < requestCount)
	{
		time_t timeTotal = 0;

		ClientSocket socket(url);

		if (!socket.isConnected()) // check connection
		{
			cout << "Could not connect to host " << url << endl;
			return 1;
		}

		// ready to make requests
		// ======================

		// initial request
		// ===============
		socket.makeRequest();
		timeBegin = time(NULL);
		socket.readResponse();
		timeEnd = time(NULL);
		timeTotal += timeEnd - timeBegin;

		// read from stream until 0 char if chunked
		if (socket.response.headers["Transfer-Encoding"] == "chunked")
		{
			while(true)
			{
				if(!socket.readResponse()) break;

				// look for 0 chunk size and break
				if (string(socket.response.buffer).find("\r\n0\r\n") != string::npos)
					break;
			}
		} else if(socket.response.headers.find("Content-Length") != socket.response.headers.end())
		{
			// get rest of response body if missing
			while(socket.response.body.size() < stoi(socket.response.headers["Content-Length"]))
				if(!socket.readResponse()) break;
		}

		// display response body on first iteration
		if(responseCount == 0)
		{
			cout  << socket.response.body << endl;

			if(requestCount > 1) // if profiling
				printMsg("Making "+to_string(requestCount-1)+" More Requests...");
		}

		responseCount++;

		timeSum += socket.response.time; // for mean

		responseList.push_back(socket.response); // store response obj
		responseTimes.push_back(socket.response.time); // store response time 

	} // end requests

	// gather profiling data
	// =====================
	if(requestCount > 1)
	{
		// extrema of response times 
		time_t fastestTime = 0;
		time_t slowestTime = 0;

		// extrema of read sizes 
		int largestRead = 0;
		int smallestRead = 0;

		// store error codes and count success
		int successfulRequests = 0; // count success
		vector<string> errCodes;
		for(auto &r: responseList)
		{
			// calc extrema of response times
			if(fastestTime == 0 && slowestTime == 0)
				fastestTime = slowestTime = r.time;
			else if(r.time < fastestTime) // fastest 
				fastestTime = r.time;
			else if(r.time > slowestTime) // s l o w
				slowestTime = r.time;


			// get read sizes
			if(largestRead == 0 && smallestRead == 0)
				largestRead = smallestRead = r.bytesRead;	
			else if(r.bytesRead > largestRead) // largest
				largestRead = r.bytesRead;	
			else if(r.bytesRead < smallestRead) // smallest
				smallestRead = r.bytesRead;	

			// get error codes and count successes
			if(!r.success) // failed
			{
				// unique error code?
 				if(find(errCodes.begin(), errCodes.end(), r.status) == errCodes.end())
					errCodes.push_back(r.status);
			}
			else
			{
				successfulRequests++;
			}

		}

		// calc success rate
		float successRate = ((float) successfulRequests)/((float) responseList.size());

		// display 
		printMsg("Profiling Results");
		cout << endl;
		printMsg("Requests made: "+to_string(responseList.size()));
		printMsg("Fastest Time: "+to_string(fastestTime)+"s");
		printMsg("Slowest Time: "+to_string(slowestTime)+"s");
		printMsg("Mean Time: "+to_string(timeSum/responseList.size())+"s");

		// for median
		sort(responseTimes.begin(),responseTimes.end());
		printMsg("Median Time: "+to_string(responseTimes[responseTimes.size()/2])+"s");

		printMsg("Success Rate: "+to_string((int) (successRate*100.0))+"%");

		printMsg("Error Codes");
		if(!errCodes.empty())
		{
			for(auto &c : errCodes) cout << c << endl;
			cout << endl;
		}
		else
		{
			cout << "No Error Codes" << endl;
		}

		printMsg("Smallest Response: "+to_string(smallestRead)+" Bytes");
		printMsg("Largest Response: "+to_string(largestRead)+" Bytes");
	}


	return 0;
}

void printMsg(string msg)
{
	cout << "[ "+msg+" ]" << endl;
}
