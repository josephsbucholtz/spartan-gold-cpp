#include <iostream>
#include "transaction.h"

int main() {
    Transaction tx {
        .from = "Alice",
        .nonce = 0,
        .pubKey = "Alice's public key",
        .fee = 10,
        .outputs = {50, 30}
    };

    int total = tx.totalOutput();

    std::cout << "Total output: " << total << "\n";
    return 0;
}