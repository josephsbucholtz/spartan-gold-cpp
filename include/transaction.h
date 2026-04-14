#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Block;

class Transaction
{
public:
    std::string from_ = "";
    std::string sig = "";
    std::string id;
    int nonce_ = 0;

    //Constructors
    Transaction(const std::string& from,
                int nonce,
                const std::string& pubKey,
                int fee,
                const std::vector<int>& outputs
            );

    Transaction(const std::string& from,
                int nonce,
                const std::string& pubKey,
                int fee,
                const std::vector<int>& outputs,
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
    int fee_ = 0;
    std::vector<int> outputs_ {};
    std::string data_ = "";
};
