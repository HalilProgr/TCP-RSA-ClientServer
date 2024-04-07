#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketStream.h>
#include "Encryptor.h"


class ClientHandler
{
private:
	std::string host;
	int port;

	Crypto::PublicKey* key;
	Crypto::Encryptor* ecnr;

	// IP endpoint/socket address (consists of host addr and port #)
	Poco::Net::SocketAddress socketAddr;

	// Interface to a TCP stream socket
	Poco::Net::StreamSocket socket;

	// Stream for reading from / writing to a socket (accepts a socket)
	Poco::Net::SocketStream stream;


	Crypto::PublicKey* deserializationPublicKey(std::string& input);

public:
	ClientHandler(std::string h, int port);
	~ClientHandler();

	bool connected();
	bool sendMessage();
};

Crypto::CryptoString StringToCryptoString(std::string& input);
std::string CryptoStringToString(Crypto::CryptoString& input);