#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <transaction.h>
#include <ctime>
#include <client.h>

class Block 
{
    public:
        Block();
        bool isGenesisBlock();
        bool hasValidProof();
        std::string seralize();
        //json toJSON(); -- return a JSON object
        std::string hashVal();
        std::string id();
        bool addTransaction(Transaction tx, Client client);
        bool rerun(Block prevBlock);
        u_int64_t balanceOf(std::string addr);
        u_int64_t totalRewards(); 
        bool contains(Transaction tx);
    
    private:
        std::shared_ptr<Block> prevBlock_ = nullptr;
        int prevBlockHash_ = 0;
        u_int64_t target_ = 0;
        std::unordered_map<Block, Block> balances_;
        std::unordered_map<Block, Block> nextNonce_;
        std::unordered_map<Transaction, Transaction> transactions_;
        int chainLength_ = 0;
        time_t timestamp_ = 0;
        std::string rewardAddr_ = "";
        u_int64_t coinbaseReward_ = 0;
};