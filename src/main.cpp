#include <iostream>
#include "transaction.h"

int main() {
    Transaction tx("Alice", 0, "Alice's public key", 10, {50, 30});

    std::cout << "Total output: " << tx.totalOutput() << "\n";
    return 0;
}