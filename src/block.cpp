#include "block.h"
#include "utils.h"

Block::Block(std::string rewardAddr, 
    Block *prevBlock, 
    BigInt target, 
    int coinbaseReward) : 
    rewardAddr_(rewardAddr),
    target_(target),
    coinbaseReward_(coinbaseReward)
{
}

bool Block::isGenesisBlock()
{
    return chainLength_ == 0;
}

bool Block::hasValidProof() 
{
    std::string h = utils::hash(this->serialize());
    BigInt hashInt = hexToBigInt(h);
    return hashInt < target_;
}

std::string Block::serialize() 
{
    return this->toJSON().dump();
}

nlohmann::ordered_json Block::toJSON()
{
    using ordered_json = nlohmann::ordered_json;

    ordered_json o;
    o["chainLength"] = this->chainLength_;

    o["timestamp"] = this->timestamp_;

    if (this->isGenesisBlock())
    {
        ordered_json balances = ordered_json::array();

        for (const auto& [addr, amount] : this->balances_)
        {
            ordered_json entry = ordered_json::array();
            entry.push_back(addr);
            entry.push_back(amount);
            balances.push_back(entry);
        }

        o["balances"] = balances;
    }
    else
    {
        ordered_json txs = ordered_json::array();

        for (const auto& [txid, tx] : this->transactions_)
        {
            ordered_json txj;
            txj["from"] = tx.getFrom();
            txj["sig"] = tx.getSig();
            txj["id"] = tx.id;
            txj["fee"] = tx.getFee();
            txj["nonce"] = tx.getNonce();
            txj["pubKey"] = tx.getPubKey();

            ordered_json outputs = ordered_json::array();
            for (const auto& out : tx.getOutputs())
            {
                ordered_json outj;
                outj["amount"] = out.amount;
                outj["address"] = out.address;
                outputs.push_back(outj);
            }
            txj["outputs"] = outputs;

            if (tx.getData().has_value())
            {
                txj["data"] = tx.getData().value();
            }

            ordered_json entry = ordered_json::array();
            entry.push_back(txid);
            entry.push_back(txj);
            txs.push_back(entry);
        }

        o["transactions"] = txs;
        o["prevBlockHash"] = this->prevBlockHash_;

        o["proof"] = this->proof_;

        o["rewardAddr"] = this->rewardAddr_;
    }

    return o;
}

std::string Block::hashVal()
{
    return utils::hash(this->serialize());
}

std::string Block::id()
{
    return Block::hashVal();
}

bool Block::addTransaction(Transaction tx)
{
    if(transactions_.find(tx.id) != transactions_.end())
    {
        return false;
    }
    if (tx.getSig() == "" || !tx.validSignature())
    {
        return false;
    }
    if (!tx.sufficientFunds(*this)) 
    {
        return false;
    }
        
    int nonce = this->nextNonces_[tx.getFrom()];

    if (tx.getNonce() < nonce) { return false; }
    else if (tx.getNonce() > nonce) { return false; }
    else {
        this->nextNonces_[tx.getFrom()] = nonce + 1;
    }

    this->transactions_.emplace(tx.id, tx);

    int senderBalance = this->balanceOf(tx.getFrom());
    this->balances_[tx.getFrom()] = senderBalance - tx.totalOutput();
    
    for (const auto& output : tx.getOutputs()) {
        this->balances_[output.address] += output.amount;
    }

    return true;
}

bool Block::rerun(Block *prevBlock)
{
    this->balances_ = prevBlock->balances_;
    this->nextNonces_ = prevBlock->nextNonces_;

    int winnerBalance = this->balanceOf(prevBlock->rewardAddr_);
    if (prevBlock->rewardAddr_ != "") this->balances_[prevBlock->rewardAddr_] = winnerBalance + prevBlock->totalRewards();

    std::unordered_map<std::string, Transaction> txs = this->transactions_;
    this->transactions_.clear();

    for (const auto& pair : txs) {
        if (!this->addTransaction(pair.second)) {
            return false;
        }
    } 

    return true;
}

uint64_t Block::balanceOf(std::string addr)
{
    return this->balances_[addr];
}

uint64_t Block::totalRewards()
{
    uint64_t reward = coinbaseReward_;

    for (const auto& [id, tx] : transactions_) {
        reward += tx.getFee();
    }

    return reward;
}

bool Block::contains(Transaction tx)
{
    return this->transactions_.find(tx.id) != this->transactions_.end(); 
}
