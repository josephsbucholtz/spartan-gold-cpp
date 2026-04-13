#include <iostream>

#include "transaction.h"
#include "utils.h"

int main() {
    Transaction tx("Alice", 0, "Alice's public key", 10, {50, 30});
    tx.id();

    std::cout << "Total output: " << tx.totalOutput() << "\n";
    std::cout << "id: " << tx.getId()  << "\n";

    return 0;
}