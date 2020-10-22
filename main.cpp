#include <iostream>
#include <string>
#include "clientsocket.h" 

using namespace std;

int main()
{
	string url = "foobiebletch.net";

	ClientSocket socket(url);
	/*
	socket.makeSocket();
	socket.setHostIP(url);
	socket.connectToHost();
	*/
	
	if(!socket.isConnected())
		cout << "Could not connect to host " << url << endl;
	else
	{
		cout << "Connected to host " << url << endl;
		cout << "Host IP: " << socket.getHostIP() << endl;
	}

	socket.makeRequest();
	cout << "\tResponse:" << endl;
	cout << socket.getResponse() << endl;

	return 0;
}
