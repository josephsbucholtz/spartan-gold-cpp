#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <cstdint>
#include <block.h>
#include <client.h>
#include <miner.h>
#include <cstdint>


class Blockchain
{
  public:
      Blockchain();
      Block makeGenesis();
      Block deserializeBlock(Block block);
      Block makeBlock(Block block); //no need?
      Transaction makeTransaction(Transaction tx); //No need?
      Blockchain getInstance();
      Blockchain hasInstance();
      //Blockchain createInstance(config); work in Progress
      void showBalances(std::string name);
      //void start(int milisec) work in progress
      Block makeBlock(Block block); //Maybe no need
      std::vector<Client> getClients(std::vector<Client> names);
      void register_clients(std::vector<Client> clients);
      std::string getClientName(std::string address);


  private:
      std::vector<std::shared_ptr<Client>> clients_;
      std::vector<std::shared_ptr<Miner>> miners_;
      std::unordered_map<std::string, std::shared_ptr<Client>> clientAddressMap_;
      std::unordered_map<std::string, std::shared_ptr<Client>> clientNameMap_;
      std::unordered_map<std::string, uint64_t> initialBalances_ = {};
      Network* net_ = nullptr;
      uint64_t powLeadingZeroes_ = 0;
      uint64_t coinbaseReward_ = 0;
      uint64_t defaultTxFee_ = 0;
      uint64_t confirmedDepth_ = 0;
      uint64_t powTarget_ = 0;
      std::string mnemonic_ = "";
      static Blockchain* instance_;
};