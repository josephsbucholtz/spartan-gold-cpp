#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <client.h>

class Network
{
public:
    // Constructors
    Network();
    Network(double chanceMessageFails,
            int messageDelayMax);

    // methods
    void register_clients(std::vector<Client> clientList);

    // Needs object : figure it out
    void broadcast(std::string msg);                     // need o
    void sendMessage(std::string addr, std::string msg); // need o

    bool recognizes(Client client);

private:
    std::unordered_map<std::string, std::shared_ptr<Client>> clients_;
    double chanceMessageFails_ = 0.0;
    int messageDelayMax_ = 0;
};