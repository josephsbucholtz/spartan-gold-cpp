#pragma once
#include <string>

namespace utils
{
    // CRYPTO SETTINGS
    std::string HASH_ALG = "sha256";
    std::string SIG_ALG = "RSA-SHA256";

    std::string hash(std::string string, std::string encoding);
    std::pair<std::string, std::string> generateKeyPairFromMnemonic(std::string mnemonic, std::string password);
    std::pair<std::string, std::string> generateKeypair();
    std::string sign(std::string privKey, std::string msg);
    bool verifySignature(std::string pubKey, std::string msg, std::string sig);
    std::string calcAddress(std::string key);
    bool addressMatchesKey(std::string addr, std::string pubKey);
}
