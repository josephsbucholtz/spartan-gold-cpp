
#include "transaction.h"
#include "block.h"
#include "utils.h"

#define TX_CONST "TX"

Transaction::Transaction(const std::string &from,
                         int nonce,
                         const std::string &pubKey,
                         int fee,
                         const std::vector<Output> &outputs)
    : from_(from), nonce_(nonce), pubKey_(pubKey), fee_(fee), outputs_(outputs), data_(std::nullopt)
{
}

Transaction::Transaction(const std::string &from,
                         int nonce,
                         const std::string &pubKey,
                         int fee,
                         const std::vector<Output> &outputs,
                         const std::string &data)
    : from_(from), nonce_(nonce), pubKey_(pubKey), fee_(fee), outputs_(outputs), data_(data)
{
}

void Transaction::getId() 
{
    id = utils::hash(TX_CONST + from_ + std::to_string(nonce_) + pubKey_ + 
    std::to_string(fee_) + this->getData().value_or("") + std::to_string(totalOutput()));
}


void Transaction::sign(std::string privKey)
{
    sig = utils::sign(privKey, "");
}

bool Transaction::validSignature()
{
    return sig.empty() == false &&
           utils::addressMatchesKey(from_, pubKey_) &&
           utils::verifySignature(pubKey_, "", sig);
}

bool Transaction::sufficientFunds(Block &block)
{
    return this->totalOutput() <= block.balanceOf(this->getFrom());
}

uint64_t Transaction::totalOutput()
{
    int total = fee_;
    for (const auto& output : outputs_) {
        total += output.amount;
    }

    if (total < 0) {
        throw std::runtime_error("Total output cannot be negative");
    }

    return total;
}