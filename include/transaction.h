#pragma once

#include <string>
#include <vector>
#include <cstdint>


class Transaction
{
public:
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
    void id();
    std::string getId() const;
    void sign(std::string privKey);
    bool validSignature();
    uint64_t totalOutput();

private:
    std::string id_;
    std::string from_ = "";
    int nonce_ = 0;
    std::string pubKey_ = "";
    std::string sig_ = "";
    int fee_ = 0;
    std::vector<int> outputs_ {};
    std::string data_ = "";
};
