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

bool Block::hasValidProof() //need serialize function
{
    return false;
}

std::string Block::serialize() //need JSON library
{
    return std::string();
}

void Block::toJSON() //need JSON library
{
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
        
    int nonce;
    if (this->nextNonces_[tx.getFrom()]) {
        nonce = this->nextNonces_[tx.getFrom()];
    } else {
        nonce = 0;
    }

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
    return this->balances_[addr] || 0 ;
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
