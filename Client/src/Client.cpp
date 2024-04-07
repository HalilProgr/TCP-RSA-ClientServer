#include "Client.h"
#include <Poco/Buffer.h>
#include <iostream>
#include <string>
#include <sstream>


ClientHandler::ClientHandler(std::string h, int p) :
	host(h), port(p), socketAddr(h, p), socket(), stream(socket)
{
	std::cout << "Host: " << host << "\tPort: " << port << std::endl;
}

// Connect to the initialized socket address' hostname and port
bool ClientHandler::connected()
{
	Poco::Buffer<char> buf(1000);
	std::cout << "Creating a connection with ["
		<< socketAddr.host().toString()
		<< "] through port [" << socketAddr.port() << "] ...";
	try {
		socket.connect(socketAddr);
		std::cout << "Success!" << std::endl;
		socket.receiveBytes(buf);
		std::string str(buf.begin(), buf.end());
		key = deserializationPublicKey(str);
		ecnr = new Crypto::Encryptor(key);
	}
	catch (Poco::Exception err) {
		std::cout << std::endl;
		std::cout << "Socket connection error: [" << err.displayText() << "]" << std::endl;
		return false;
	}
	return true;
}

ClientHandler::~ClientHandler()
{
	delete key;
	delete ecnr;
}

// Send a message to the connected server
bool ClientHandler::sendMessage()
{
	std::cout << std::endl;

	try
	{
		std::string message;
		std::cout << "Enter a message to send to the server: ";
		std::getline(std::cin, message);
		if (message.compare("exit") != 0)
		{
			auto temp = ecnr->encryptString(message);
			std::string cryptoString = CryptoStringToString(temp);
			std::cout << "Message \"" << message  << "\" to the server!" << std::endl;
			std::cout << "Sending the crypto message \"" << cryptoString << "\" to the server!" << std::endl;
			socket.sendBytes(cryptoString.data(), cryptoString.size());
			return true;
		}
		else
		{
			message = "exit";
			socket.sendBytes(message.data(), message.size());
			return false;
		}
	}
	catch (Poco::Exception err)
	{
		std::cout << "Data send error: [" << err.displayText() << "]" << std::endl;
		return false;
	}
}

Crypto::PublicKey* ClientHandler::deserializationPublicKey(std::string& input)
{
	std::istringstream f(input);
	std::string s;
	int256_t m, r;
	std::getline(f, s, ';');
	m = std::stoi(s);
	std::getline(f, s, ';');
	r = std::stoi(s);
	std::cout << m << " " << r << std::endl;
	Crypto::PublicKey* res = new Crypto::PublicKey{ r, m };
	return res;
}

Crypto::CryptoString StringToCryptoString(std::string& input)
{
	Crypto::CryptoString result;
	std::istringstream f(input);
	string s;
	while (std::getline(f, s, ';')) {
		result.push_back(std::stoi(s));
	}

	return result;
}

std::string CryptoStringToString(Crypto::CryptoString& input)
{
	std::string result;
	for (auto& ch : input)
		result += ch.str() + ";";
	return result;
}