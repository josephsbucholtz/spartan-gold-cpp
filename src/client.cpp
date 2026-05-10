#include "client.h"

#include "blockchain.h"
#include "network.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <stdexcept>

Client::Client(const std::string &name,
               Blockchain *bc)
    : name_(name), bc_(bc)
{
    if (bc_ == nullptr)
    {
        throw std::invalid_argument("Client requires a valid Blockchain pointer");
    }

    net_ = bc_->getNetwork();
    generateAddress("");
}

void Client::setGenesisBlock(const Block &genesis)
{
    genesis_ = genesis;
    latestBlock_ = genesis;
    hasLatestBlock_ = true;
    blocks_[genesis.id()] = genesis;
}

void Client::generateAddress(const std::string &mnemonic)
{
    (void)mnemonic;

    keyPair_ = utils::generateKeyPair();
    address_ = utils::calcAddress(keyPair_.publicKey);

    std::cout << name_ << "'s address is: " << address_ << "\n";
}

void Client::showAllBalances() const
{
    if (bc_ == nullptr)
    {
        std::cout << "No blockchain attached.\n";
        return;
    }

    const auto &clients = bc_->getClients();

    std::cout << "Balances from " << name_ << "'s perspective:\n";

    const Block &viewBlock = hasLatestBlock_ ? latestBlock_ : genesis_;

    for (const auto &client : clients)
    {
        if (!client)
            continue;

        std::cout
            << "  " << client->getName()
            << " (" << client->getAddress() << ")"
            << ": " << viewBlock.balanceOf(client->getAddress())
            << "\n";
    }
}

void Client::postTransaction(const std::vector<Output> &outputs,
                             const std::string &data)
{
    if (bc_ == nullptr)
    {
        throw std::runtime_error("Client is not attached to a blockchain");
    }

    const int nonce = nonces_[address_];
    const int fee = static_cast<int>(bc_->getDefaultTxFee());

    Transaction tx = data.empty()
                         ? Transaction(address_, nonce, keyPair_.publicKey, fee, outputs)
                         : Transaction(address_, nonce, keyPair_.publicKey, fee, outputs, data);

    tx.getId();
    tx.sign(keyPair_.privateKey);

    pendingTxs_.push_back(tx);
    nonces_[address_]++;

    std::cout << name_ << " posted transaction " << tx.id << "\n";

    if (net_ != nullptr)
    {
        // net_->broadcast(Blockchain::POST_TRANSACTION, tx.id, address_);
        net_->broadcast(Blockchain::POST_TRANSACTION, tx.toJSON().dump(), address_);
    }
}

void Client::receive(const std::string &msgType,
                     const std::string &payload,
                     const std::string &from)
{
    std::cout
        << "[" << name_ << "] received "
        << msgType
        << " from " << from
        << " payload=" << payload
        << "\n";

    if (msgType == Blockchain::POST_TRANSACTION) // NEEDS WORK
    {
    }
    else if (msgType == Blockchain::PROOF_FOUND)
    {
        receiveBlock(payload);
    }
    else if (msgType == Blockchain::MISSING_BLOCK)
    {
    }
    else if (msgType == Blockchain::START_MINING)
    {
    }
}

Block *Client::receiveBlock(const std::string &payload)
{
    Block block = bc_->deserializeBlock(nlohmann::ordered_json::parse(payload));
    const std::string blockId = block.hashVal();

    if (blocks_.count(blockId))
    {
        return nullptr;
    }

    if (!block.hasValidProof() && !block.isGenesisBlock())
    {
        std::cout << getName() << ": block has invalid proof\n";
        return nullptr;
    }

    Block *prevBlock = nullptr;

    if (!block.isGenesisBlock())
    {
        std::string prevHash = block.getPrevBlockHash();

        if (!blocks_.count(prevHash))
        {
            std::cout << getName() << ": missing previous block "
                      << prevHash << "\n";
            return nullptr;
        }

        prevBlock = &blocks_.at(prevHash);

        if (!block.rerun(prevBlock))
        {
            std::cout << getName() << ": block rerun failed\n";
            return nullptr;
        }
    }

    blocks_[blockId] = block;
    Block *storedBlock = &blocks_.at(blockId);

    if (!hasLatestBlock_ ||
        storedBlock->getChainLength() > latestBlock_.getChainLength())
    {
        latestBlock_ = *storedBlock;
        hasLatestBlock_ = true;
    }

    return storedBlock;
}