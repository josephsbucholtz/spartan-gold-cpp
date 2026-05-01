#include <iostream>
#include <thread>

#include "transaction.h"
#include "utils.h"
#include "block.h"
#include "blockchain.h"
#include "client.h"
#include "miner.h"
#include "fake-net.h"

void driver()
{
    std::cout << "---------Starting simulation. This may take a moment...----------\n";

    Blockchain bc;
    auto net = std::make_shared<FakeNet>();
    bc.setNetwork(net.get());

    auto alice   = std::make_shared<Client>("Alice", &bc);
    auto bob     = std::make_shared<Client>("Bob", &bc);
    auto charlie = std::make_shared<Client>("Charlie", &bc);
    auto minnie  = std::make_shared<Miner>("Minnie", &bc, 2000);
    auto mickey  = std::make_shared<Miner>("Mickey", &bc, 2000);

    net->registerClient(alice);
    net->registerClient(bob);
    net->registerClient(charlie);
    net->registerClient(minnie);
    net->registerClient(mickey);

    bc.registerClient(alice);
    bc.registerClient(bob);
    bc.registerClient(charlie);
    bc.registerMiner(minnie);
    bc.registerMiner(mickey);

    bc.setInitialBalance(alice->getAddress(), 233);
    bc.setInitialBalance(bob->getAddress(), 99);
    bc.setInitialBalance(charlie->getAddress(), 67);
    bc.setInitialBalance(minnie->getAddress(), 400);
    bc.setInitialBalance(mickey->getAddress(), 300);

    Block genesis = bc.makeGenesis();
    bc.setGenesis(genesis);

    alice->setGenesisBlock(genesis);
    bob->setGenesisBlock(genesis);
    charlie->setGenesisBlock(genesis);
    minnie->setGenesisBlock(genesis);
    mickey->setGenesisBlock(genesis);

    std::cout << "------------ INITIAL BALANCES --------------\n";
    alice->showAllBalances();

    minnie->init();
    mickey->init();

    std::cout << "Alice is transferring 40 gold to " << bob->getAddress() << "\n";
    alice->postTransaction({{40, bob->getAddress()}});

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\n------------ Starting a late-to-the-party miner --------------\n";
    auto donald = std::make_shared<Miner>("Donald", &bc, 3000);

    net->registerClient(donald);
    bc.registerMiner(donald);
    donald->setGenesisBlock(bc.getGenesis());
    donald->init();

    std::this_thread::sleep_for(std::chrono::seconds(6));

    std::cout << "\n------------ Final Balances: Alice perspective --------------\n";
    alice->showAllBalances();
}


int main()
{
    driver();
    return 0;
}