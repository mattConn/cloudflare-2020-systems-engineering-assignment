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

		// read headers,status and body into response obj
		Response response(socket.getRawResponse());

		// read from stream until 0 char if chunked
		if (response.headers["Transfer-Encoding"] == "chunked")
		{
			int chunkCount = 1;
			while(true)
			{
				cout << "\033[H\033[J"; // clear screen
				printGreen("Reading Chunked Response "+to_string(responseCount+1)+"/"+to_string(requestCount));
				printGreen("Waiting For Chunk "+to_string(chunkCount+1));

				timeBegin = time(NULL);
				if(!socket.readResponse()) break;
				timeEnd = time(NULL);

				printGreen("Received Chunk "+to_string(chunkCount+1));
				cout << endl;

				timeTotal += timeEnd - timeBegin;

				response.body += socket.getRawResponse();
				response.body.pop_back(); // chomp
				response.body.pop_back(); // and again

				chunkCount++;
				// look for 0 chunk size and break
				if (socket.getRawResponse().find("\r\n0\r\n") != string::npos)
					break;
			}
		} else if(response.headers.find("Content-Length") != response.headers.end())
		{
			// get rest of response body if missing
			while(response.body.size() < stoi(response.headers["Content-Length"]))
			{
			socket.readResponse();
			response.body += socket.getRawResponse();
			response.body.pop_back(); // chomp
			response.body.pop_back(); // and again
			}
		}

		// display response body on first iteration
		if(responseCount == 0)
		{
			cout  << response.body << endl;

			if(requestCount > 1) // if profiling
				printGreen("Making "+to_string(requestCount)+" Requests...");
		}

		responseCount++;

		// store total byte size
		response.bytesRead = socket.getTotalBytesRead();
		// store response time
		response.time = timeTotal;
		responseTimes.push_back(timeTotal);
		timeSum += timeTotal; // for mean

		responseList.push_back(response); // store response obj

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
		printGreen("Profiling Results");
		cout << endl;
		printGreen("Requests made: "+to_string(responseList.size()));
		printGreen("Fastest Time: "+to_string(fastestTime)+"s");
		printGreen("Slowest Time: "+to_string(slowestTime)+"s");
		printGreen("Mean Time: "+to_string(timeSum/responseList.size())+"s");

		// for median
		sort(responseTimes.begin(),responseTimes.end());
		printGreen("Median Time: "+to_string(responseTimes[responseTimes.size()/2])+"s");

		printGreen("Success Rate: "+to_string((int) (successRate*100.0))+"%");

		printGreen("Error Codes");
		if(!errCodes.empty())
		{
			for(auto &c : errCodes) cout << c << endl;
			cout << endl;
		}
		else
		{
			cout << "No Error Codes" << endl;
		}

		printGreen("Smallest Response: "+to_string(smallestRead)+" Bytes");
		printGreen("Largest Response: "+to_string(largestRead)+" Bytes");
	}


	return 0;
}

void printGreen(string msg)
{
	cout << "\033[1;32m"
		 << "[ " << msg << " ]"
		 << "\033[0m" << endl;
}
