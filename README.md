# SpartanGold++

Port of Educational Cryptocurrency Spartan Gold from Javascript to C++ 

Link to original project: https://github.com/taustin/spartan-gold 

# Description - per original project
SpartanGold (SG) is a simplified blockchain-based cryptocurrency for education and experimentation.

Its design is loosely based on Bitcoin. Like Bitcoin, SpartanGold uses a proof-of-work (PoW) blockchain. However, several parts of the design are simplified. For example:

SpartanGold uses an account-based model, rather than Bitcoin's unspent transaction output (UTXO) model.
No scripting language is used. Transactions are only designed to transfer money (gold in SG parlance).
The proof-of-work target is not adjusted during program execution.
All of these features could be added to SpartanGold, but we want to make the design as simple and easy to modify as possible.

# Run Instructions (CMake)
We used CMake and the clang compiler to build the project. Here are run instructions for the project.

### 1. Create Build Directory
```bash
cmake -S . -B build
```

### 2. Compile
```bash
cmake --build build
```
### 3. Run
```bash
./build/spartan_gold_app
```
