
#include "transaction.h"
#include "utils.h"
#include "block.h"

Transaction::Transaction()
{
}

Transaction::Transaction(const std::string from, uint64_t nonce,
                         const std::string pubKey,
                         uint64_t fee,
                         const std::vector<int> outputs)
{
    from_ = from;
    nonce_ = nonce;
    pubKey_ = pubKey;
    fee_ = fee;
    outputs_ = outputs;
}

Transaction Transaction::id()
{
    return Transaction();
}


void Transaction::sign(std::string privKey)
{
    sig_ = utils::sign(privKey, "");
}

bool Transaction::validSignature()
{
    return sig_.empty() == false && 
    utils::addressMatchesKey(from_, pubKey_) &&
    utils::verifySignature(pubKey_, "", sig_);
}

bool Transaction::sufficientFunds(Block block)
{
    return this->totalOutput() <= block.balances_[from_];
}

uint64_t Transaction::totalOutput()
{
    uint64_t total = fee_;
    for (size_t i = 0; i < outputs_.size(); ++i) {
        total += outputs_[i];
    }
    return total;
}