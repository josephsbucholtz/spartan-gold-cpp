#pragma once

#include <block.h>
#include <transaction.h>
#include <client.h>
#include <unordered_set>

class Miner : public Client
{
public:
    //Constructors
    Miner();
    Miner(const std::string &name,
          const std::string &password,
          const std::string &address,
          Network *net,
          uint64_t nonce = 0);


    //methods
    void initialize();
    void startNewSearch(std::unordered_set<Transaction> txSet);
    void findProof(bool oneAndDone = false);
    void announceProof();
    Block receivedBlock(Block s);
    std::unordered_set<Transaction> syncTransactions(Block newBlock);
    void addTransaction(Transaction tx);
    void postTransaction(); // needs work

private:
    int miningRounds_ = 0;
    std::unordered_set<Transaction> transactions_;
    Block currentBlock_ {};
};