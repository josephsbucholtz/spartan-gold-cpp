#include "network.h"

Network::Network()
{
}

Network::Network(double chanceMessageFails, int messageDelayMax)
{
}

void Network::register_clients(std::vector<Client> clientList)
{
}

void Network::broadcast(std::string msg)
{
}

void Network::sendMessage(std::string addr, std::string msg)
{
}

bool Network::recognizes(Client client)
{
    return false;
}
