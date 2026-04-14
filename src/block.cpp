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
    if (tx.sig == "" || !tx.validSignature())
    {
        return false;
    }
    if (!tx.sufficientFunds(*this)) 
    {
        return false;
    }

    int nonce = this->nextNonces_[tx.from_];

    return true;
}

bool Block::rerun(Block *prevBlock)
{
    return false;
}

uint64_t Block::balanceOf(std::string addr)
{
    return 0;
}
