#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

class Block;

struct Output {
    uint64_t amount;
    std::string address;
};

class Transaction
{
public:
    std::string id;

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
    bool sufficientFunds(Block& block);
    uint64_t totalOutput();
    
    //getters
    std::string getFrom() const { return from_; }
    std::string getPubKey() const { return pubKey_; }
    std::string getSig() const { return sig; }
    std::vector<Output> getOutputs() const { return outputs_; }
    int getNonce() const { return nonce_; }
    std::optional<std::string> getData() const { return data_; }
    int getFee() const { return fee_; }

private:
    std::string from_ = "";
    std::string sig = "";
    int fee_ = 0;
    std::vector<Output> outputs_ {};
    int nonce_ = 0;
    std::string pubKey_ = "";
    std::optional<std::string> data_ = std::nullopt;
};
