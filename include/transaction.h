#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Block;

struct Output {
    uint64_t amount;
    std::string address;
};

class Transaction
{
public:
    std::string from_ = "";
    std::string sig = "";
    std::string id;
    int fee_ = 0;
    std::vector<Output> outputs_ {};
    int nonce_ = 0;

    //Constructors
    Transaction(const std::string& from,
                int nonce,
                const std::string& pubKey,
                int fee,
                const std::vector<Output>& outputs
            );

    Transaction(const std::string& from,
                int nonce,
                const std::string& pubKey,
                int fee,
                const std::vector<Output>& outputs,
                const std::string& data 
            );
                
    // methods
    void getId();
    void sign(std::string privKey);
    bool validSignature();
    bool sufficientFunds(Block block);
    uint64_t totalOutput();

private:
    std::string pubKey_ = "";
    std::string data_ = "";
};
