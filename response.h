#include <map>
#include <string>

using namespace std;

// storing and handling response string
struct Response
{
	string status;				 // status line
	map<string, string> headers; // header lines
	string body;				 // body of request
	int bytesRead;				// bytes read from response

	// constructors
	Response(){};		// default
	Response(string r); // with response str

	bool setHeaders(string rawResponse); // get data from response str
};
