#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include <string>
#include <thread>
#include "src/Server.h"


int main(int argc, char* argv[]) {
    // Create the server socket to listen
    Poco::Net::ServerSocket socket(8000);

    // Configure some server parameters
    Poco::Net::TCPServerParams* pParams = new Poco::Net::TCPServerParams();
    pParams->setMaxThreads(4);
    pParams->setMaxQueued(4);
    pParams->setThreadIdleTime(100);

    // Create the server
    Poco::Net::TCPServer myserver(
        new Poco::Net::TCPServerConnectionFactoryImpl<newConnection>(), socket,
        pParams);
    myserver.start();

    while (true)
        ;

    return 0;
}