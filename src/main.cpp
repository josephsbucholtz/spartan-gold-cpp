#include <iostream>

#include "transaction.h"
#include "utils.h"

int main() {
    Transaction tx("Alice", 0, "Alice's public key", 10, {{20, "ffff"}, {40, "face"}});
    tx.getId();

    std::cout << "Total output: " << tx.totalOutput() << "\n";
    std::cout << "id: " << tx.id  << "\n";

    return 0;
}