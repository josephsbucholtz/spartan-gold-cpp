#pragma once
#include <memory>
#include <string>

class Client;

class Network
{
public:
    virtual ~Network() = default;
    virtual void registerClient(const std::shared_ptr<Client>& client) = 0;
    virtual void broadcast(const std::string& msgType,
                           const std::string& payload,
                           const std::string& fromAddress = "") = 0;
    virtual void sendMessage(const std::string& to,
                          const std::string& msgType,
                          const std::string& payload,
                          const std::string& from) = 0;
};