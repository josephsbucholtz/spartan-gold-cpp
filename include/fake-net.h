#pragma once
#include "network.h"
#include <map>
#include <memory>
#include <string>

class FakeNet : public Network
{
public:
    void registerClient(const std::shared_ptr<Client>& client) override;
    void broadcast(const std::string& msgType,
                   const std::string& payload,
                   const std::string& fromAddress = "") override;
    void sendMessage(const std::string& to,
                          const std::string& msgType,
                          const std::string& payload,
                          const std::string& from) override;
private:
    std::map<std::string, std::shared_ptr<Client>> clients_;
};