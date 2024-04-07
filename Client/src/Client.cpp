#include "Client.h"
#include <Poco/Buffer.h>
#include <iostream>
#include <string>
#include <sstream>


namespace Net
{

	ClientHandler::ClientHandler(std::string h, int p) :
		_host(h), _port(p), _socketAddr(h, p), _socket()
	{
		std::cout << "Host: " << _host << "\tPort: " << _port << std::endl;
	}

	// Connect to the initialized socket address' hostname and port
	bool ClientHandler::connected()
	{
		Poco::Buffer<char> buf(1000);
		std::cout << "Creating a connection with ["
			<< _socketAddr.host().toString()
			<< "] through port [" << _socketAddr.port() << "] ...";
		try {
			_socket.connect(_socketAddr);
			std::cout << "Success!" << std::endl;
			_socket.receiveBytes(buf);
			std::string str(buf.begin(), buf.end());
			_key = std::make_unique<Crypto::PublicKey>(deserializationPublicKey(str));
			_ecnr = std::make_unique<Crypto::Encryptor>(_key.get());
		}
		catch (Poco::Exception err) {
			std::cout << std::endl;
			std::cout << "Socket connection error: [" << err.displayText() << "]" << std::endl;
			return false;
		}
		return true;
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
				auto temp = _ecnr->encryptString(message);
				std::string cryptoString = CryptoStringToString(temp);
				std::cout << "Message \"" << message << "\" to the server!" << std::endl;
				std::cout << "Sending the crypto message \"" << cryptoString << "\" to the server!" << std::endl;
				_socket.sendBytes(cryptoString.data(), cryptoString.size());
				return true;
			}
			else
			{
				message = "exit";
				_socket.sendBytes(message.data(), message.size());
				return false;
			}
		}
		catch (Poco::Exception err)
		{
			std::cout << "Data send error: [" << err.displayText() << "]" << std::endl;
			return false;
		}
	}

	Crypto::PublicKey ClientHandler::deserializationPublicKey(std::string& input)
	{
		std::istringstream f(input);
		std::string s;
		int256_t m, r;
		std::getline(f, s, ';');
		m = std::stoi(s);
		std::getline(f, s, ';');
		r = std::stoi(s);
		std::cout << m << " " << r << std::endl;
		return Crypto::PublicKey{ r, m };
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
}