#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <ctime>

class Client;
class Transaction;

class Block
{
public:
    std::unordered_map<std::string, uint64_t> balances_ {};

    //Constructors
    Block();
    Block(std::shared_ptr<Block> prevBlock,
          uint64_t target,
          const std::string &rewardAddr,
          uint64_t coinbaseReward);


    //methods
    bool isGenesisBlock();
    bool hasValidProof();
    std::string serialize();
    // json toJSON(); -- return a JSON object
    std::string hashVal();
    std::string id();
    // bool addTransaction(Transaction tx, Client client);
    bool rerun(Block prevBlock);
    uint64_t balanceOf(std::string addr);
    uint64_t totalRewards();
    // bool contains(Transaction tx);

private:
    std::shared_ptr<Block> prevBlock_ = nullptr;
    int prevBlockHash_ = 0;
    uint64_t target_ = 0;
    std::unordered_map<std::string, uint64_t> nextNonce_ {};
    std::unordered_map<std::string, Transaction> transactions_;
    int chainLength_ = 0;
    time_t timestamp_ = 0;
    std::string rewardAddr_ = "";
    uint64_t coinbaseReward_ = 0;
};
