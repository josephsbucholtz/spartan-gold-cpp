
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
    sig = utils::sign(privKey, id);
}

bool Transaction::validSignature()
{
    return sig.empty() == false &&
           utils::addressMatchesKey(from_, pubKey_) &&
           utils::verifySignature(pubKey_, id, sig);
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

nlohmann::ordered_json Transaction::toJSON() const
{
    nlohmann::ordered_json j;

    j["from"] = from_;
    j["nonce"] = nonce_;
    j["pubKey"] = pubKey_;
    j["fee"] = fee_;
    j["sig"] = sig;
    j["id"] = id;

    nlohmann::ordered_json outputs = nlohmann::ordered_json::array();

    for (const auto& out : outputs_) {
        nlohmann::ordered_json outj;
        outj["amount"] = out.amount;
        outj["address"] = out.address;
        outputs.push_back(outj);
    }

    j["outputs"] = outputs;

    if (data_.has_value()) {
        j["data"] = data_.value();
    }

    return j;
}

Transaction Transaction::fromJSON(const nlohmann::ordered_json& j)
{
    std::vector<Output> outputs;

    for (const auto& outj : j["outputs"]) {
        Output out;
        out.amount = outj["amount"].get<uint64_t>();
        out.address = outj["address"].get<std::string>();
        outputs.push_back(out);
    }

    Transaction tx(
        j.value("from", ""),
        j.value("nonce", 0),
        j.value("pubKey", ""),
        j.value("fee", 0),
        outputs
    );

    if (j.contains("data")) {
        tx = Transaction(
            j.value("from", ""),
            j.value("nonce", 0),
            j.value("pubKey", ""),
            j.value("fee", 0),
            outputs,
            j["data"].get<std::string>()
        );
    }

    tx.id = j.value("id", "");
    tx.setSig(j.value("sig", ""));

    return tx;
}

