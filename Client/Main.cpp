#include <string>
#include "src/Client.h"


int main(int argc, char* argv[])
{
	int port = 8000;
	std::string hostname = "127.0.0.1";

	Net::ClientHandler handler(hostname, port);
	if (handler.connected())
		while (handler.sendMessage());

	return 0;
}