#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <cstdint>

#include "client.h"
#include "transaction.h"
#include "block.h"

class Miner;
class Network;

class Blockchain
{
public:
    Blockchain();
    Blockchain(uint64_t powLeadingZeroes,
               uint64_t coinbaseReward,
               uint64_t defaultTxFee,
               uint64_t confirmedDepth,
               uint64_t powTarget,
               const std::string& mnemonic,
               Network* net);

    Block makeGenesis();
    Block deserializeBlock(const Block& block);
    Block makeBlock(const Block& block);
    Transaction makeTransaction(const Transaction& tx);

    static Blockchain& getInstance();
    static bool hasInstance();

    void showBalances(const std::string& name);
    std::vector<Client> getClients(const std::vector<Client>& names);
    void register_clients(const std::vector<Client>& clients);
    std::string getClientName(const std::string& address);

private:
    std::vector<std::shared_ptr<Client>> clients_;
    std::vector<std::shared_ptr<Miner>> miners_;
    std::unordered_map<std::string, std::shared_ptr<Client>> clientAddressMap_;
    std::unordered_map<std::string, std::shared_ptr<Client>> clientNameMap_;
    std::unordered_map<std::string, uint64_t> initialBalances_{};
    Network* net_ = nullptr;
    uint64_t powLeadingZeroes_ = 0;
    uint64_t coinbaseReward_ = 0;
    uint64_t defaultTxFee_ = 0;
    uint64_t confirmedDepth_ = 0;
    uint64_t powTarget_ = 0;
    std::string mnemonic_ = "";
    static Blockchain* instance_;
};