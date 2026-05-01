#include "blockchain.h"

#include "client.h"
#include "miner.h"

#include <iostream>
#include <stdexcept>

Blockchain::Blockchain()
    : net_(nullptr),
      powLeadingZeroes_(5),
      coinbaseReward_(25),
      defaultTxFee_(1),
      confirmedDepth_(6),
      powTarget_(0)
{
}
Blockchain::Blockchain(const std::vector<std::shared_ptr<Client>> &clients, Network *net)
    : clients_(clients), net_(net),
      powLeadingZeroes_(5),
      coinbaseReward_(25),
      defaultTxFee_(1),
      confirmedDepth_(6),
      powTarget_(0)
{
    powTarget_ = hexToBigInt("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    for (const auto &client : clients_)
    {
        if (!client)
            continue;

        clientNameMap_[client->getName()] = client;

        if (!client->getAddress().empty())
        {
            clientAddressMap_[client->getAddress()] = client;
        }

        // For now you can assign placeholder starting balances here,
        // or later load them from config.
        initialBalances_[client->getAddress()] = 100;
    }

    genesis_ = makeGenesis();

    for (const auto &client : clients_)
    {
        if (client)
        {
            client->setGenesisBlock(genesis_.value());
        }
    }
}

Block Blockchain::makeGenesis() const
{
    return Block(initialBalances_, powTarget_, static_cast<int>(coinbaseReward_));
}

Block Blockchain::deserializeBlock(const nlohmann::ordered_json &j) const
{
    (void)j;
    throw std::runtime_error("Blockchain::deserializeBlock() not implemented yet");
}

Block Blockchain::makeBlock(const Block &prevBlock, const std::string &rewardAddr) const
{
    Block* prevPtr = const_cast<Block*>(&prevBlock);
    return Block(rewardAddr, prevPtr, powTarget_, static_cast<int>(coinbaseReward_));
}

Transaction Blockchain::makeTransaction(const std::string &from,
                                        int nonce,
                                        const std::string &pubKey,
                                        int fee,
                                        const std::vector<Output> &outputs) const
{
    return Transaction(from, nonce, pubKey, fee, outputs);
}

void Blockchain::registerClient(const std::shared_ptr<Client> &client)
{
    if (!client)
    {
        return;
    }

    clients_.push_back(client);
    clientNameMap_[client->getName()] = client;

    if (!client->getAddress().empty())
    {
        clientAddressMap_[client->getAddress()] = client;
    }
}

void Blockchain::registerMiner(const std::shared_ptr<Miner> &miner)
{
    if (!miner)
    {
        return;
    }

    miners_.push_back(miner);

    // Since Miner is-a Client, also register in the client collections.
    clients_.push_back(miner);
    clientNameMap_[miner->getName()] = miner;

    if (!miner->getAddress().empty())
    {
        clientAddressMap_[miner->getAddress()] = miner;
    }
}

void Blockchain::showBalances(const std::string &label) const
{
    if (!label.empty())
    {
        std::cout << label << "\n";
    }

    std::cout << "Known clients:\n";
    for (const auto &client : clients_)
    {
        if (!client)
            continue;

        std::cout << "  " << client->getName()
                  << " -> " << client->getAddress()
                  << "\n";
    }
}

void Blockchain::setInitialBalance(const std::string& address, uint64_t amount)
{
    initialBalances_[address] = amount;
}

void Blockchain::start()
{
    for (const auto& miner : miners_)
    {
        if (miner)
        {
            miner->init();
        }
    }
}

std::shared_ptr<Client> Blockchain::getClientByAddress(const std::string &address) const
{
    auto it = clientAddressMap_.find(address);
    if (it == clientAddressMap_.end())
    {
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<Client> Blockchain::getClientByName(const std::string &name) const
{
    auto it = clientNameMap_.find(name);
    if (it == clientNameMap_.end())
    {
        return nullptr;
    }
    return it->second;
}

const std::vector<std::shared_ptr<Client>> &Blockchain::getClients() const
{
    return clients_;
}

const std::vector<std::shared_ptr<Miner>> &Blockchain::getMiners() const
{
    return miners_;
}

void Blockchain::setNetwork(Network *net)
{
    net_ = net;
}

Network *Blockchain::getNetwork() const
{
    return net_;
}

const std::map<std::string, uint64_t> &Blockchain::getInitialBalances() const
{
    return initialBalances_;
}

uint64_t Blockchain::getCoinbaseReward() const
{
    return coinbaseReward_;
}

uint64_t Blockchain::getDefaultTxFee() const
{
    return defaultTxFee_;
}

uint64_t Blockchain::getConfirmedDepth() const
{
    return confirmedDepth_;
}

uint64_t Blockchain::getPowLeadingZeroes() const
{
    return powLeadingZeroes_;
}

const BigInt &Blockchain::getPowTarget() const
{
    return powTarget_;
}

void Blockchain::setGenesis(const Block &block)
{
    genesis_ = block;
}

const Block &Blockchain::getGenesis() const
{
    return genesis_.value();
}
