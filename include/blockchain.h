#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "block.h"
#include "transaction.h"

class Client;
class Miner;
class Network;

class Blockchain
{
public:
    //constants
    static constexpr const char* MISSING_BLOCK   = "MISSING_BLOCK";
    static constexpr const char* POST_TRANSACTION = "POST_TRANSACTION";
    static constexpr const char* PROOF_FOUND     = "PROOF_FOUND";
    static constexpr const char* START_MINING    = "START_MINING";

    //constructors
    Blockchain();
    Blockchain(const std::vector<std::shared_ptr<Client>>& clients, Network* net);
    Blockchain(const std::vector<std::shared_ptr<Client>>& clients, const std::string& mnemonic, Network* net);

    Block makeGenesis() const;
    Block deserializeBlock(const nlohmann::ordered_json& j) const;
    Block makeBlock(const Block& prevBlock, const std::string& rewardAddr) const;
    Transaction makeTransaction(const std::string& from,
                                int nonce,
                                const std::string& pubKey,
                                int fee,
                                const std::vector<Output>& outputs) const;

    void registerClient(const std::shared_ptr<Client>& client);
    void registerMiner(const std::shared_ptr<Miner>& miner);
    void showBalances(const std::string& label = "") const;

    //getters and setters
    std::shared_ptr<Client> getClientByAddress(const std::string& address) const;
    std::shared_ptr<Client> getClientByName(const std::string& name) const;
    const std::vector<std::shared_ptr<Client>>& getClients() const;
    const std::vector<std::shared_ptr<Miner>>& getMiners() const;
    void setNetwork(Network* net);
    Network* getNetwork() const;
    const std::map<std::string, uint64_t>& getInitialBalances() const;
    uint64_t getCoinbaseReward() const;
    uint64_t getDefaultTxFee() const;
    uint64_t getConfirmedDepth() const;
    uint64_t getPowLeadingZeroes() const;
    const std::string& getMnemonic() const;
    const BigInt& getPowTarget() const;

private:
    std::vector<std::shared_ptr<Client>> clients_;
    std::vector<std::shared_ptr<Miner>> miners_;
    std::map<std::string, std::shared_ptr<Client>> clientAddressMap_;
    std::map<std::string, std::shared_ptr<Client>> clientNameMap_;

    std::map<std::string, uint64_t> initialBalances_{};
    Network* net_ = nullptr;

    uint64_t powLeadingZeroes_ = 5;
    uint64_t coinbaseReward_ = 25;
    uint64_t defaultTxFee_ = 1;
    uint64_t confirmedDepth_ = 6;
    BigInt powTarget_;
    std::string mnemonic_;
};