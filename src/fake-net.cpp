#include "fake-net.h"
#include "client.h"

void FakeNet::registerClient(const std::shared_ptr<Client> &client)
{
    clients_.insert({client->getAddress(), client});
}

void FakeNet::broadcast(const std::string &msgType,
                        const std::string &payload,
                        const std::string &fromAddress)
{
    for (auto& [addr, client] : clients_)
    {
        if (addr == fromAddress)
            continue;

        client->receive(msgType, payload, fromAddress);
    }
}

void FakeNet::sendMessage(const std::string& to,
                          const std::string& msgType,
                          const std::string& payload,
                          const std::string& from)
{
    auto it = clients_.find(to);
    if (it != clients_.end())
    {
        it->second->receive(msgType, payload, from);
    }
}