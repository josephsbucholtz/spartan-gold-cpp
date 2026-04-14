#pragma once

#include <string>
#include <unordered_map>

#include "transaction.h"
#include <boost/multiprecision/cpp_int.hpp>

using BigInt = boost::multiprecision::cpp_int;

inline BigInt hexToBigInt(const std::string& hex)
{
    BigInt result = 0;

    std::size_t start = 0;
    if (hex.size() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        start = 2;
    }

    for (std::size_t i = start; i < hex.size(); ++i) {
        result <<= 4;
        char c = hex[i];

        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'a' && c <= 'f') {
            result += 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            result += 10 + (c - 'A');
        } else {
            throw std::runtime_error("Invalid hex string");
        }
    }

    return result;
}

class Block 
{
    static inline const BigInt POW_TARGET =
        hexToBigInt("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    static constexpr int COINBASE_AMT_ALLOWED = 25;

    public:
    // Constructors 
        Block(std::string rewardAddr,
              Block* prevBlock,
              BigInt target=POW_TARGET,
              int coinbaseReward=COINBASE_AMT_ALLOWED);

    // Methods
        bool isGenesisBlock();
        bool hasValidProof();
        std::string serialize();
        void toJSON(); //needs to return a JSON object
        std::string hashVal();
        std::string id();
        bool addTransaction(Transaction tx); //need to add client object when implemented
        bool rerun(Block* prevBlock);
        uint64_t balanceOf(std::string addr);
        uint64_t totalRewards();
        bool contains(Transaction tx);

    private:
        std::string prevBlockHash_ = "";
        BigInt target_ = 0;
        std::unordered_map<std::string, int> balances_ {};
        std::unordered_map<std::string, int> nextNonces_ {};
        std::unordered_map<std::string, Transaction> transactions_ {};
        int chainLength_ = 0;
        // long timestamp_ = 0;
        std::string rewardAddr_ = "";
        int coinbaseReward_ = 0;
};