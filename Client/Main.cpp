#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Thread.h"
#include "Poco/RunnableAdapter.h"
#include <string>
#include <thread>
#include "src/Client.h"

int main(int argc, char* argv[])
{
	int port = 8000;
	std::string hostname = "127.0.0.1";

	// Handle the server-client connection and send some json
	ClientHandler handler(hostname, port);
	if (handler.connected())
		while (handler.sendMessage());

	return 0;
}