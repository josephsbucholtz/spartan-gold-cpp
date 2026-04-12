#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "block.h"

class Transaction
{
public:
    //Constructors
    Transaction();
    Transaction(const std::string from, uint64_t nonce, const std::string pubKey, uint64_t fee, const std::vector<int> outputs);

    // methods
    Transaction id();
    void sign(std::string privKey);
    bool validSignature();
    bool sufficientFunds(Block block);
    uint64_t totalOutput();

private:
    std::string from_ = "";
    uint64_t nonce_ = 0;
    std::string pubKey_ = "";
    std::string sig_ = "";
    uint64_t fee_ = 0;
    std::vector<int> outputs_ {};
    // add data field when we know what it is
};
