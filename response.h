#include <map>
#include <string>
#include <time.h>

using namespace std;

// storing and handling response string
struct Response
{
	string status;				 // status line
	bool success = true;		  // depends on status
	map<string, string> headers; // header lines
	string body;				 // body of request
	int bytesRead = 0;				// bytes read from response
	time_t time = 0;					// response time

	// constructors
	Response(){};		// default
	Response(string r); // with response str

	bool setHeadersAndBody(string rawResponse); // get data from response str
};
