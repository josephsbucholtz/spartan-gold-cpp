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

    std::cout << name_ << ": Showing balances:\n";

    const Block &viewBlock = hasLatestBlock_ ? latestBlock_ : genesis_;

    for (const auto &client : clients)
    {
        if (!client)
            continue;

        std::cout << "    " << client->getAddress()
          << " (" << client->getName() << "): "
          << viewBlock.balanceOf(client->getAddress()) << "\n";
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
        net_->broadcast(Blockchain::POST_TRANSACTION, tx.toJSON().dump(), address_);
    }
}

void Client::receive(const std::string &msgType,
                     const std::string &payload,
                     const std::string &from)
{

    if (msgType == Blockchain::POST_TRANSACTION)
    {
        // Regular clients can ignore mempool
    }
    else if (msgType == Blockchain::PROOF_FOUND)
    {
        receiveBlock(payload);
    }
    else if (msgType == Blockchain::MISSING_BLOCK)
    {
        const std::string requestedHash = payload;

        auto it = blocks_.find(requestedHash);
        if (it == blocks_.end())
        {
            std::cout << getName()
                      << ": requested missing block not known "
                      << requestedHash << "\n";
            return;
        }

        if (net_ != nullptr)
        {
            std::cout << getName()
                      << " sending missing block "
                      << requestedHash
                      << " to " << from << "\n";

            net_->sendMessage(
                from,
                Blockchain::PROOF_FOUND,
                it->second.serialize(),
                getAddress()
            );
        }
    }
    else if (msgType == Blockchain::START_MINING)
    {
    }
}

Block *Client::receiveBlock(const std::string &payload)
{
    Block block = bc_->deserializeBlock(nlohmann::ordered_json::parse(payload));
    const std::string blockId = block.id();

    if (blocks_.count(blockId))
    {
        std::cout << getName() << ": duplicate block\n";
        return nullptr;
    }

    if (!block.hasValidProof() && !block.isGenesisBlock())
    {
        std::cout << getName() << ": block has invalid proof\n";
        return nullptr;
    }

    if (!block.isGenesisBlock())
    {
        const std::string prevHash = block.getPrevBlockHash();

        if (!blocks_.count(prevHash))
        {
            std::cout << getName()
                      << ": missing previous block "
                      << prevHash << "\n";

            orphanPayloads_[blockId] = payload;
            orphansByPrev_.emplace(prevHash, blockId);

            if (net_ != nullptr && !requestedBlocks_.count(prevHash))
            {
                requestedBlocks_.insert(prevHash);

                net_->broadcast(
                    Blockchain::MISSING_BLOCK,
                    prevHash,
                    getAddress()
                );
            }

            return nullptr;
        }

        Block *prevBlock = &blocks_.at(prevHash);

        if (!block.rerun(prevBlock))
        {
            std::cout << getName() << ": block rerun failed\n";
            return nullptr;
        }
    }

    blocks_[blockId] = block;
    Block *storedBlock = &blocks_.at(blockId);

    bool becameLatest = false;

    if (!hasLatestBlock_ ||
        storedBlock->getChainLength() > latestBlock_.getChainLength())
    {
        latestBlock_ = *storedBlock;
        hasLatestBlock_ = true;
        becameLatest = true;

    }

    requestedBlocks_.erase(blockId);

    std::vector<std::string> childPayloads;

    auto range = orphansByPrev_.equal_range(blockId);
    for (auto it = range.first; it != range.second; ++it)
    {
        auto orphanIt = orphanPayloads_.find(it->second);
        if (orphanIt != orphanPayloads_.end())
        {
            childPayloads.push_back(orphanIt->second);
            orphanPayloads_.erase(orphanIt);
        }
    }
    orphansByPrev_.erase(blockId);

    for (const std::string &childPayload : childPayloads)
    {
        receiveBlock(childPayload);
    }

    return storedBlock;
}