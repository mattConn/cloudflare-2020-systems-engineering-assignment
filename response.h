#include <map>
#include <string>

using namespace std;

// storing and handling response string
struct Response {
	string status; // status line
	map<string,string> headers; // header lines
	string body; // body of request

	// constructors
	Response(){}; // default
	Response(string r); // with response str

	bool setHeaders(string rawResponse); // get data from response str
};
