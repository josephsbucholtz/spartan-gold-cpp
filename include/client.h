#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "transaction.h"
#include "block.h"
#include "utils.h"

class Network;
class Blockchain;

class Client
{
public:
    // Constructor
    Client(const std::string& name,
           Blockchain* bc);

    virtual ~Client() = default;

    // Core behavior
    void setGenesisBlock(const Block& genesis);
    void generateAddress(const std::string& mnemonic);
    void showAllBalances() const;

    void postTransaction(const std::vector<Output>& outputs,
                         const std::string& data = "");

    // Network interface
    virtual void receive(const std::string& msgType,
                         const std::string& payload,
                         const std::string& from);

    // Getters
    std::string getAddress() const { return address_; }
    std::string getName() const { return name_; }
    std::string getPubKey() const { return keyPair_.publicKey; }

protected:
    // Identity
    std::string name_;
    std::string address_;

    // Crypto
    utils::RSAKeyPair keyPair_;

    // Blockchain context
    Blockchain* bc_ = nullptr;
    Network* net_ = nullptr;

    // State
    Block genesis_;
    std::map<std::string, Block> blocks_;   // known blocks by hash
    std::map<std::string, int> nonces_;     // nonce per address
    Block latestBlock_;
    bool hasLatestBlock_ = false;

    // Pending transactions
    std::vector<Transaction> pendingTxs_;
};