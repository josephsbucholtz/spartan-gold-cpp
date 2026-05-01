#pragma once

#include <memory>
#include <string>
#include <vector>

#include "client.h"
#include "block.h"
#include "transaction.h"

class Blockchain;

class Miner : public Client
{
public:
    // Constructor
    Miner(const std::string& name,
          Blockchain* bc,
          int miningRounds = 2000);

    virtual ~Miner() = default;

    // Entry point (called by Blockchain / driver)
    void init();

    // Mining loop (single-threaded stub for now)
    void startNewSearch();

    // Proof-of-work attempt
    bool findProof();

    // Announce a found block
    void announceProof();

    // Override network receive
    void receive(const std::string& msgType,
                 const std::string& payload,
                 const std::string& from) override;

protected:
    // Current block being mined
    Block currentBlock_;

    // Transactions to include
    std::vector<Transaction> mempool_;

    // Mining parameters
    int miningRounds_ = 2000;

    // Internal helpers
    void addTransaction(const Transaction& tx);
};