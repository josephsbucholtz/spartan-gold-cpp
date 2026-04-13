
#include "transaction.h"
#include "utils.h"

#define TX_CONST "TX"

Transaction::Transaction(const std::string &from,
                         int nonce,
                         const std::string &pubKey,
                         int fee,
                         const std::vector<int> &outputs)
    : from_(from), nonce_(nonce), pubKey_(pubKey), fee_(fee), outputs_(outputs)
{
}

Transaction::Transaction(const std::string &from,
                         int nonce,
                         const std::string &pubKey,
                         int fee,
                         const std::vector<int> &outputs,
                         const std::string &data)
    : from_(from), nonce_(nonce), pubKey_(pubKey), fee_(fee), outputs_(outputs), data_(data)
{
}

void Transaction::id() 
{
    id_ = utils::hash(TX_CONST + from_ + std::to_string(nonce_) + pubKey_ + std::to_string(fee_) + data_);
}

std::string Transaction::getId() const
{
    return id_;
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

uint64_t Transaction::totalOutput()
{
    int total = fee_;
    for (size_t i{}; i < outputs_.size(); ++i)
    {
        total += outputs_[i];
    }
    return total;
}