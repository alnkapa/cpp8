#include "hash.h"

#include <openssl/evp.h>

#include <cstdio>
#include <stdexcept>

namespace hash {
    HashImpl::HashImpl(hash::Algorithm algorithm) : m_algorithm(algorithm) {};

    HashImpl::HashImpl(const EVP_MD* hash, hash::Algorithm algorithm) : m_hash(hash), m_algorithm(algorithm) {};

    HashImpl::~HashImpl() {}

    std::unique_ptr<HashImpl> HashImpl::Create(hash::Algorithm in) {
        switch (in) {
            case hash::Algorithm::SHA256:
                return std::make_unique<SHA256>();
            case hash::Algorithm::MD5:
                return std::make_unique<MD5>();
            case hash::Algorithm::SHA1:
                return std::make_unique<SHA1>();
            case hash::Algorithm::CRC32:
                return std::make_unique<SHA1>();
            default:
                throw std::invalid_argument("Unknown hash type");
        }
    }

    HashType HashImpl::hash(const HashType& in) {
        // хеш-значения
        unsigned char m_value[EVP_MAX_MD_SIZE]{0};
        // хеш-длина
        unsigned int m_len{0};
        // Освобождаем контекст по выходу
        struct EVP_MD_CTX_Deleter {
            void operator()(EVP_MD_CTX* context) const { EVP_MD_CTX_free(context); }
        };

        // контекст для хеширования
        std::unique_ptr<EVP_MD_CTX, EVP_MD_CTX_Deleter> context(EVP_MD_CTX_new());
        if (!context) {
            throw std::runtime_error("Failed to create EVP_MD_CTX");
        }

        // инициализация
        if (EVP_DigestInit_ex(context.get(), m_hash, NULL) != 1) {
            throw std::runtime_error("Failed to initialize digest");
        }

        // хеширование
        if (EVP_DigestUpdate(context.get(), in.c_str(), in.length()) != 1) {
            throw std::runtime_error("Failed to update digest");
        }

        // результат
        if (EVP_DigestFinal_ex(context.get(), m_value, &m_len) != 1) {
            throw std::runtime_error("Failed to finalize digest");
        }

        return {reinterpret_cast<const char*>(&m_value), std::size_t(m_len)};
    }

    hash::Algorithm HashImpl::getAlgorithm() const noexcept { return m_algorithm; }

    SHA256::SHA256() : HashImpl(EVP_sha256(), hash::Algorithm::SHA256) {}
    MD5::MD5() : HashImpl(EVP_md5(), hash::Algorithm::MD5) {};
    SHA1::SHA1() : HashImpl(EVP_sha1(), hash::Algorithm::SHA1) {};

}  // namespace hash