#pragma once

#include <iostream>
#include <cstring>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/TCPServerConnection.h>
#include "RSA.h"


class newConnection : public Poco::Net::TCPServerConnection {

public:
    newConnection(const Poco::Net::StreamSocket& s)
        : Poco::Net::TCPServerConnection(s), _rsa(11, 17, 3) {}
    ~newConnection();

    void run() override;

private:
    std::string serializationPublicKey(const Crypto::PublicKey* key);

    Crypto::RSA _rsa;
};

Crypto::CryptoString StringToCryptoString(std::string& input);
std::string CryptoStringToString(Crypto::CryptoString& input);