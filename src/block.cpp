#include "block.h"

Block::Block(std::shared_ptr<Block> prevBlock, uint64_t target, const std::string &rewardAddr, uint64_t coinbaseReward)
{
}

bool Block::isGenesisBlock()
{
    return false;
}

bool Block::hasValidProof()
{
    return false;
}

std::string Block::serialize()
{
    return std::string();
}

std::string Block::hashVal()
{
    return std::string();
}

std::string Block::id()
{
    return std::string();
}



bool Block::rerun(Block prevBlock)
{
    return false;
}

uint64_t Block::balanceOf(std::string addr)
{
    return uint64_t();
}

uint64_t Block::totalRewards()
{
    return uint64_t();
}
