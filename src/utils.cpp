#include "utils.h"

namespace utils
{

    std::string hash(const std::string &string, const std::string &encoding)
    {
        return "";
    }

    // std::pair<std::string, std::string> utils::generateKeyPairFromMnemonic(const std::string &mnemonic, const std::string &password)
    // {
    //     return {};
    // }

    std::pair<std::string, std::string> generateKeypair()
    {
        return {};
    }

    std::string sign(std::string privKey, std::string msg)
    {
        return "";
    }

    bool verifySignature(std::string pubKey, std::string msg, std::string sig)
    {
        return false;
    }

    std::string calcAddress(std::string key)
    {
        return std::string();
    }

    bool addressMatchesKey(std::string addr, std::string pubKey)
    {
        return false;
    }

} // namespace utils
