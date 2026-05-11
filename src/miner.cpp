#include "miner.h"

#include "blockchain.h"
#include "network.h"

#include <iostream>

Miner::Miner(const std::string &name,
             Blockchain *bc,
             int miningRounds)
    : Client(name, bc),
      miningRounds_(miningRounds)
{
}

void Miner::init()
{
    std::cout << getName() << " miner initialized\n";
    startNewSearch();
}

void Miner::startNewSearch()
{
    // std::cout << getName() << " starting new search\n";
    std::cout << getName()
              << " starting new search on length "
              << latestBlock_.getChainLength()
              << " mempool=" << mempool_.size()
              << "\n";

    currentBlock_ = bc_->makeBlock(latestBlock_, getAddress());

    std::vector<Transaction> stillPending;

    for (const Transaction &tx : mempool_)
    {
        bool added = currentBlock_.addTransaction(tx);

        std::cout << getName()
                  << " add tx " << tx.id
                  << " added=" << added
                  << "\n";

        if (!added)
        {
            stillPending.push_back(tx);
        }
    }

    mempool_ = stillPending;
    currentBlock_.setProof(0);
}

bool Miner::findProof()
{
    uint64_t pausePoint = currentBlock_.getProof() + miningRounds_;

    while (currentBlock_.getProof() < pausePoint)
    {
        if (currentBlock_.hasValidProof())
        {
            std::cout << getName()
                      << " found block length "
                      << currentBlock_.getChainLength()
                      << " txs="
                      << currentBlock_.transactionCount()
                      << "\n";

            announceProof();

            Block *accepted = Client::receiveBlock(currentBlock_.serialize());
            if (accepted != nullptr)
            {
                startNewSearch();
            }

            return true;
        }
        currentBlock_.incrementProof();
    }
    return false;
}

void Miner::announceProof()
{
    if (net_)
    {
        net_->broadcast(Blockchain::PROOF_FOUND, currentBlock_.serialize(), getAddress());
    }
}

void Miner::receive(const std::string &msgType,
                    const std::string &payload,
                    const std::string &from)
{
    std::cout << "[" << getName() << " / miner] received "
              << msgType << " from " << from
              << " payload=" << payload << "\n";

    if (msgType == Blockchain::POST_TRANSACTION)
    {
        // For now, just acknowledge receipt.
        // Later:
        // - deserialize the tx
        // - validate it
        // - add it to mempool_
        try
        {
            auto json = nlohmann::ordered_json::parse(payload);
            Transaction tx = Transaction::fromJSON(json);

            mempool_.push_back(tx);

            std::cout << getName()
                      << " queued transaction " << tx.id
                      << " mempool=" << mempool_.size()
                      << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << getName() << " failed to parse transaction: "
                      << e.what() << "\n";
        }
    }
    else if (msgType == Blockchain::START_MINING)
    {
        startNewSearch();
    }
    else if (msgType == Blockchain::PROOF_FOUND)
    {
        // Later:
        // - validate the incoming block
        // - switch chain view if appropriate
        std::cout << getName() << " saw proof announcement\n";
        receiveBlock(payload);
    }
    else if (msgType == Blockchain::MISSING_BLOCK)
    {
        // Later:
        // - return requested block if known
    }
    else
    {
        // Fall back to base client behavior if you want shared logging/handling.
        Client::receive(msgType, payload, from);
    }
}

Block *Miner::receiveBlock(const std::string &payload)
{
    int oldLength = latestBlock_.getChainLength();

    Block *b = Client::receiveBlock(payload);

    if (b == nullptr)
    {
        return nullptr;
    }

    if (latestBlock_.getChainLength() > oldLength &&
        latestBlock_.getChainLength() >= currentBlock_.getChainLength())
    {
        std::cout << getName() << " cutting over to new chain\n";
        startNewSearch();
    }

    return b;
}

void Miner::addTransaction(const Transaction &tx)
{
    mempool_.push_back(tx);
}