#include "miner.h"

#include "blockchain.h"
#include "network.h"

#include <iostream>

Miner::Miner(const std::string& name,
             Blockchain* bc,
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
    std::cout << getName() << " starting new search\n";

    // Minimal placeholder behavior for now.
    // Later this should:
    // - build a candidate block from the latest known block
    // - add mempool transactions
    // - set reward address
    // - reset proof search state
}

bool Miner::findProof()
{
    std::cout << getName() << " attempting proof search for "
              << miningRounds_ << " rounds\n";

    // Minimal stub:
    // return false for now until block creation / proof mutation exists.
    return false;
}

void Miner::announceProof()
{
    std::cout << getName() << " announcing proof\n";

    if (net_ == nullptr)
    {
        return;
    }

    // Minimal placeholder payload.
    // Later this should be the serialized mined block.
    net_->broadcast(Blockchain::PROOF_FOUND, "proof_found", getAddress());
}

void Miner::receive(const std::string& msgType,
                    const std::string& payload,
                    const std::string& from)
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
        std::cout << getName() << " queued transaction payload\n";
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

void Miner::addTransaction(const Transaction& tx)
{
    mempool_.push_back(tx);
}