#pragma once

#include <string_view>
#include <utility>
#include <string>


namespace utils
{
    // CRYPTO SETTINGS
    inline constexpr std::string_view HASH_ALG = "SHA-256";
    inline constexpr std::string_view SIG_ALG  = "RSA-SHA256";

    std::string hash(const std::string& string, const std::string& encoding="hex");
    std::pair<std::string, std::string> generateKeyPairFromMnemonic(const std::string& mnemonic, const std::string& password);
    std::pair<std::string, std::string> generateKeypair();
    std::string sign(std::string privKey, std::string msg);
    bool verifySignature(std::string pubKey, std::string msg, std::string sig);
    std::string calcAddress(std::string key);
    bool addressMatchesKey(std::string addr, std::string pubKey);
}
