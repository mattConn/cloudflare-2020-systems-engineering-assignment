#include <iostream>
#include <string>
#include "clientsocket.h" 

using namespace std;

int main()
{
	string url = "foobiebletch.net";
	ClientSocket socket;
	socket.URLToIP(url);
	socket.makeSocket();
	socket.connectToHost();
	
	if(!socket.isConnected())
		cout << "Could not connect to host " << url << endl;
	else
	{
		cout << "Connected to host " << url << endl;
		cout << "Host IP: " << socket.getHostIPString() << endl;
	}
	


	return 0;
}
