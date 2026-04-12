#include "blockchain.h"

Blockchain::Blockchain()
{
}

Blockchain::Blockchain(uint64_t powLeadingZeroes, uint64_t coinbaseReward, uint64_t defaultTxFee, uint64_t confirmedDepth, uint64_t powTarget, const std::string &mnemonic, Network *net)
{
}

Block Blockchain::makeGenesis()
{
    return Block();
}

Block Blockchain::deserializeBlock(const Block &block)
{
    return Block();
}

Block Blockchain::makeBlock(const Block &block)
{
    return Block();
}

Transaction Blockchain::makeTransaction(const Transaction &tx)
{
    return Transaction();
}

bool Blockchain::hasInstance()
{
    return false;
}

void Blockchain::showBalances(const std::string &name)
{
}

std::vector<Client> Blockchain::getClients(const std::vector<Client> &names)
{
    return std::vector<Client>();
}

void Blockchain::register_clients(const std::vector<Client> &clients)
{
}

std::string Blockchain::getClientName(const std::string &address)
{
    return std::string();
}