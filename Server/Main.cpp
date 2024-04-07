#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "src/Server.h"


int main(int argc, char* argv[]) {
    Poco::Net::ServerSocket socket(8000);

    Poco::Net::TCPServerParams* pParams = new Poco::Net::TCPServerParams();
    pParams->setMaxThreads(4);
    pParams->setMaxQueued(4);
    pParams->setThreadIdleTime(100);

    Poco::Net::TCPServer myserver(
        new Poco::Net::TCPServerConnectionFactoryImpl<Net::newConnection>(), socket,
        pParams);
    myserver.start();

    while (true)
        ;

    return 0;
}