#include <iostream>

#include "transaction.h"
#include "utils.h"

void driver() {
    // std::cout << "---------Starting simulation.  This may take a moment...----------\n";  

    // Blockchain bc ( clients, net );


    // std::cout << "------------ INTIAL BALANCES--------------\n";
    // alice.showAllbalances();

    // // The miners will start mining blocks when start is called.  After 8 seconds,
    // // the code will terminate and show the final balances from Alice's perspective.

    // //WAIT SOME TIME -- <chrono> 2s 
    // Miner donald("donald" , bc.getGenesis() , 3000 );
    // std::cout << "------------Starting a late-to-the-party miner--------------\n";
    // bc.register(donald);
    // donald.init();

    // alice.postTransaction([{ amount: 40, address: bob.address }]);


    // //WAIT SOME TIME -- <chrono> 8s 
    // std::cout << "------------ Final Balances: Alice perspective: --------------\n";
    // alice.showAllbalances();
}

int main()
{
    driver();
    return 0;
}