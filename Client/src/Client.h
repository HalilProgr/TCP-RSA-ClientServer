#pragma once

#include <iostream>
#include <memory>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketStream.h>
#include "Encryptor.h"


namespace Net
{

	class ClientHandler
	{
	public:
		ClientHandler(std::string h, int port);

		bool connected();
		bool sendMessage();

	private:
		Crypto::PublicKey deserializationPublicKey(std::string& input);

		std::string _host;
		int _port;

		std::unique_ptr<Crypto::PublicKey> _key;
		std::unique_ptr<Crypto::Encryptor> _ecnr;

		Poco::Net::SocketAddress _socketAddr;
		Poco::Net::StreamSocket _socket;
	};

	Crypto::CryptoString StringToCryptoString(std::string& input);
	std::string CryptoStringToString(Crypto::CryptoString& input);
}