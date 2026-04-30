#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "utils.h"
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

namespace utils
{
    namespace
    {
        struct EVP_MD_CTX_Deleter
        {
            void operator()(EVP_MD_CTX* p) const { EVP_MD_CTX_free(p); }
        };

        struct EVP_PKEY_Deleter
        {
            void operator()(EVP_PKEY* p) const { EVP_PKEY_free(p); }
        };

        struct EVP_PKEY_CTX_Deleter
        {
            void operator()(EVP_PKEY_CTX* p) const { EVP_PKEY_CTX_free(p); }
        };

        struct BIO_Deleter
        {
            void operator()(BIO* p) const { BIO_free_all(p); }
        };

        using EVP_MD_CTX_ptr = std::unique_ptr<EVP_MD_CTX, EVP_MD_CTX_Deleter>;
        using EVP_PKEY_ptr = std::unique_ptr<EVP_PKEY, EVP_PKEY_Deleter>;
        using EVP_PKEY_CTX_ptr = std::unique_ptr<EVP_PKEY_CTX, EVP_PKEY_CTX_Deleter>;
        using BIO_ptr = std::unique_ptr<BIO, BIO_Deleter>;

        std::string toHex(const unsigned char* data, std::size_t len)
        {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (std::size_t i = 0; i < len; ++i)
            {
                oss << std::setw(2) << static_cast<unsigned int>(data[i]);
            }
            return oss.str();
        }

        std::vector<unsigned char> fromHex(const std::string& hex)
        {
            if (hex.size() % 2 != 0)
            {
                throw std::invalid_argument("hex string must have even length");
            }

            std::vector<unsigned char> out;
            out.reserve(hex.size() / 2);

            for (std::size_t i = 0; i < hex.size(); i += 2)
            {
                unsigned int byte = std::stoul(hex.substr(i, 2), nullptr, 16);
                out.push_back(static_cast<unsigned char>(byte));
            }

            return out;
        }

        std::string toBase64(const unsigned char* data, std::size_t len)
        {
            BIO* b64raw = BIO_new(BIO_f_base64());
            BIO* memraw = BIO_new(BIO_s_mem());

            if (!b64raw || !memraw)
            {
                if (b64raw) BIO_free_all(b64raw);
                if (memraw) BIO_free_all(memraw);
                throw std::runtime_error("failed to allocate BIO for base64");
            }

            BIO_ptr b64(b64raw);
            BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
            BIO_push(b64.get(), memraw);

            if (BIO_write(b64.get(), data, static_cast<int>(len)) <= 0)
            {
                throw std::runtime_error("BIO_write failed for base64");
            }

            if (BIO_flush(b64.get()) != 1)
            {
                throw std::runtime_error("BIO_flush failed for base64");
            }

            BUF_MEM* mem = nullptr;
            BIO_get_mem_ptr(b64.get(), &mem);

            if (!mem || !mem->data)
            {
                throw std::runtime_error("failed to read base64 BIO buffer");
            }

            return std::string(mem->data, mem->length);
        }

        std::vector<unsigned char> sha256Bytes(const std::string& input)
        {
            std::vector<unsigned char> digest(SHA256_DIGEST_LENGTH);
            SHA256(reinterpret_cast<const unsigned char*>(input.data()),
                   input.size(),
                   digest.data());
            return digest;
        }

        EVP_PKEY_ptr loadPrivateKeyPem(const std::string& pem)
        {
            BIO* raw = BIO_new_mem_buf(pem.data(), static_cast<int>(pem.size()));
            if (!raw)
            {
                throw std::runtime_error("failed to create BIO for private key");
            }

            EVP_PKEY* key = PEM_read_bio_PrivateKey(raw, nullptr, nullptr, nullptr);
            BIO_free_all(raw);

            if (!key)
            {
                throw std::runtime_error("failed to parse private key PEM");
            }

            return EVP_PKEY_ptr(key);
        }

        EVP_PKEY_ptr loadPublicKeyPem(const std::string& pem)
        {
            BIO* raw = BIO_new_mem_buf(pem.data(), static_cast<int>(pem.size()));
            if (!raw)
            {
                throw std::runtime_error("failed to create BIO for public key");
            }

            EVP_PKEY* key = PEM_read_bio_PUBKEY(raw, nullptr, nullptr, nullptr);
            BIO_free_all(raw);

            if (!key)
            {
                throw std::runtime_error("failed to parse public key PEM");
            }

            return EVP_PKEY_ptr(key);
        }

        std::string writePrivateKeyPem(EVP_PKEY* key)
        {
            BIO* raw = BIO_new(BIO_s_mem());
            if (!raw)
            {
                throw std::runtime_error("failed to create BIO for private key output");
            }

            BIO_ptr bio(raw);

            if (PEM_write_bio_PrivateKey(bio.get(), key, nullptr, nullptr, 0, nullptr, nullptr) != 1)
            {
                throw std::runtime_error("failed to write private key PEM");
            }

            BUF_MEM* mem = nullptr;
            BIO_get_mem_ptr(bio.get(), &mem);

            if (!mem || !mem->data)
            {
                throw std::runtime_error("failed to read private key PEM buffer");
            }

            return std::string(mem->data, mem->length);
        }

        std::string writePublicKeyPem(EVP_PKEY* key)
        {
            BIO* raw = BIO_new(BIO_s_mem());
            if (!raw)
            {
                throw std::runtime_error("failed to create BIO for public key output");
            }

            BIO_ptr bio(raw);

            if (PEM_write_bio_PUBKEY(bio.get(), key) != 1)
            {
                throw std::runtime_error("failed to write public key PEM");
            }

            BUF_MEM* mem = nullptr;
            BIO_get_mem_ptr(bio.get(), &mem);

            if (!mem || !mem->data)
            {
                throw std::runtime_error("failed to read public key PEM buffer");
            }

            return std::string(mem->data, mem->length);
        }
    }

    std::string hash(const std::string& string, const std::string& encoding)
    {
        const auto digest = sha256Bytes(string);

        if (encoding == "hex")
        {
            return toHex(digest.data(), digest.size());
        }

        if (encoding == "base64")
        {
            return toBase64(digest.data(), digest.size());
        }

        throw std::invalid_argument("unsupported hash encoding: " + encoding);
    }

    // RSAKeyPair generateKeyPairFromMnemonic(const std::string& mnemonic, const std::string& password)
    // {
    //     (void)mnemonic;
    //     (void)password;

    //     throw std::runtime_error(
    //         "generateKeyPairFromMnemonic cannot be implemented compatibly with "
    //         "the JS Spartan-Gold project using OpenSSL alone. "
    //         "The JS version depends on bip39 + node-forge seeded PRNG + node-forge RSA generation. "
    //         "Use a Node helper for exact interoperability, or port that exact algorithm."
    //     );
    // }

    RSAKeyPair generateKeyPair()
    {
        EVP_PKEY_CTX_ptr ctx(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr));
        if (!ctx)
        {
            throw std::runtime_error("failed to create RSA keygen context");
        }

        if (EVP_PKEY_keygen_init(ctx.get()) <= 0)
        {
            throw std::runtime_error("EVP_PKEY_keygen_init failed");
        }

        if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx.get(), 512) <= 0)
        {
            throw std::runtime_error("failed to set RSA modulus length");
        }

        EVP_PKEY* rawKey = nullptr;
        if (EVP_PKEY_keygen(ctx.get(), &rawKey) <= 0)
        {
            throw std::runtime_error("RSA key generation failed");
        }

        EVP_PKEY_ptr key(rawKey);

        RSAKeyPair kp;
        kp.publicKey = writePublicKeyPem(key.get());
        kp.privateKey = writePrivateKeyPem(key.get());
        return kp;
    }

    std::string sign(std::string privKey, std::string msg)
    {
        auto key = loadPrivateKeyPem(privKey);

        EVP_MD_CTX_ptr ctx(EVP_MD_CTX_new());
        if (!ctx)
        {
            throw std::runtime_error("failed to create signing context");
        }

        if (EVP_DigestSignInit(ctx.get(), nullptr, EVP_sha256(), nullptr, key.get()) <= 0)
        {
            throw std::runtime_error("EVP_DigestSignInit failed");
        }

        if (EVP_DigestSignUpdate(ctx.get(), msg.data(), msg.size()) <= 0)
        {
            throw std::runtime_error("EVP_DigestSignUpdate failed");
        }

        std::size_t sigLen = 0;
        if (EVP_DigestSignFinal(ctx.get(), nullptr, &sigLen) <= 0)
        {
            throw std::runtime_error("EVP_DigestSignFinal size query failed");
        }

        std::vector<unsigned char> sig(sigLen);
        if (EVP_DigestSignFinal(ctx.get(), sig.data(), &sigLen) <= 0)
        {
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        sig.resize(sigLen);
        return toHex(sig.data(), sig.size());
    }

    bool verifySignature(std::string pubKey, std::string msg, std::string sig)
    {
        auto key = loadPublicKeyPem(pubKey);
        auto sigBytes = fromHex(sig);

        EVP_MD_CTX_ptr ctx(EVP_MD_CTX_new());
        if (!ctx)
        {
            throw std::runtime_error("failed to create verification context");
        }

        if (EVP_DigestVerifyInit(ctx.get(), nullptr, EVP_sha256(), nullptr, key.get()) <= 0)
        {
            throw std::runtime_error("EVP_DigestVerifyInit failed");
        }

        if (EVP_DigestVerifyUpdate(ctx.get(), msg.data(), msg.size()) <= 0)
        {
            throw std::runtime_error("EVP_DigestVerifyUpdate failed");
        }

        const int rc = EVP_DigestVerifyFinal(ctx.get(), sigBytes.data(), sigBytes.size());
        return rc == 1;
    }

    std::string calcAddress(std::string key)
    {
        return hash(key, "base64");
    }

    bool addressMatchesKey(std::string addr, std::string pubKey)
    {
        return addr == calcAddress(pubKey);
    }

} // namespace utils