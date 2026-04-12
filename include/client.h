#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <utility>
#include <cstdint>

#include "block.h"
#include "transaction.h"

class Network;

class Client
{
public:
    // Constructors
    Client();
    Client(const std::string &name,
           const std::string &password,
           const std::string &address,
           Network *net,
           uint64_t nonce = 0);

    // methods
    void setGenesisBlock(Block startingBlock);
    uint64_t confirmedBalance();
    uint64_t availableGold();
    Transaction postTransaction(std::vector<std::string> outputs, uint64_t fee);
    Transaction postGenericTransaction(std::pair<std::string, std::string> txData);
    Block receivedBLock(Block block);
    void requestMisingBlock(Block block);
    void resendPendingTransactions();
    void provideMissingBlock(std::string msg);
    void setLastConfirmed();
    void showAllBalances();
    void log(std::string msg);
    void showBlockchain();
    void generateAddress(std::string mnemonic);

private:
    std::string name_ = "";
    std::string password_ = "";
    std::string address_ = "";
    Network *net_ = nullptr;
    uint64_t nonce_ = 0;
    std::unordered_map<std::string, std::shared_ptr<Transaction>> pendingOutgoingTransactions_{};
    std::unordered_map<std::string, std::shared_ptr<Transaction>> pendingReceivedTransactions_{};
    std::unordered_map<std::string, std::shared_ptr<Block>> blocks_{};
    std::unordered_map<std::string, std::vector<std::shared_ptr<Block>>> pendingBlocks_{};

    // (Optional) keypair if you add it later
    // KeyPair keyPair_;
};