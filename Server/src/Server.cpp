#include "Server.h"
#include <string>
#include <Poco/Error.h>


namespace Net
{

    void newConnection::run()
    {
        Poco::Net::StreamSocket& ss = socket();
        try
        {
            Poco::Buffer<char> buffer(0);
            std::string str = serializationPublicKey(_rsa.getPublicKey());;
            ss.sendBytes(str.data(), str.size());

            while (!ss.available())
            {
                buffer.clear();
                str.clear();

                ss.receiveBytes(buffer);
                std::copy(buffer.begin(), buffer.end(), std::inserter(str, str.begin()));

                if (str.compare("exit") == 0) break;

                if (str.find(';') != std::string::npos)
                    std::cout << "recived: " << _rsa.decrypt(StringToCryptoString(str)) << std::endl;
            }
        }
        catch (Poco::Exception& exc) {
            std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
        }
    }

    std::string newConnection::serializationPublicKey(const Crypto::PublicKey* key)
    {
        std::string keyStr = key->m.str() + ";" + key->r.str() + '\0';
        std::cout << key << std::endl;
        return keyStr;
    }

    Crypto::CryptoString StringToCryptoString(std::string& input)
    {
        Crypto::CryptoString result;
        std::istringstream f(input);
        string s;
        while (std::getline(f, s, ';'))
            result.push_back(std::stoi(s));

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