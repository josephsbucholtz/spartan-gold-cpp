
#include "utils.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

namespace utils
{

    std::string hash(const std::string &string, const std::string &encoding)
    {
        return "";
    }

    // RSAKeyPair utils::generateKeyPairFromMnemonic(const std::string &mnemonic, const std::string &password)
    // {
    //     return {};
    // }

    RSAKeyPair generateKeyPair()
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
        return hash(key);
    }

    bool addressMatchesKey(std::string addr, std::string pubKey)
    {
        return addr == calcAddress(pubKey);
    }

} // namespace utils
