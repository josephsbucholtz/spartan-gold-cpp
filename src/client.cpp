#include "client.h"

Client::Client(const std::string &name, const std::string &password, const std::string &address, Network *net, uint64_t nonce)
{
}

void Client::setGenesisBlock(Block startingBlock)
{
}

uint64_t Client::confirmedBalance()
{
    return 0;
}

uint64_t Client::availableGold()
{
    return 0;
}

Transaction Client::postTransaction(std::vector<std::string> outputs, uint64_t fee)
{
    return Transaction();
}

Transaction Client::postGenericTransaction(std::pair<std::string, std::string> txData)
{
    return Transaction();
}

Block Client::receivedBLock(Block block)
{
    return Block();
}

void Client::requestMisingBlock(Block block)
{
}

void Client::resendPendingTransactions()
{
}

void Client::provideMissingBlock(std::string msg)
{
}

void Client::setLastConfirmed()
{
}

void Client::showAllBalances()
{
}

void Client::log(std::string msg)
{
}

void Client::showBlockchain()
{
}

void Client::generateAddress(std::string mnemonic)
{
}
