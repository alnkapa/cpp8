#ifndef HASH_H
#define HASH_H
#include <openssl/evp.h>
#include <boost/crc.hpp>
#include "../global/hash.h"
#include "hash_type.h"

namespace hash
{

template <typename HashTypeImpl, Algorithm Alg> class HashImpl
{
  private:
    // хеш-функция
    const EVP_MD *m_hash;

  public:
    HashImpl()
    {
        if constexpr (Alg == Algorithm::SHA256)
        {
            m_hash = EVP_sha256();
        }
        else if constexpr (Alg == Algorithm::MD5)
        {
            m_hash = EVP_md5();
        }
        else if constexpr (Alg == Algorithm::SHA1)
        {
            m_hash = EVP_sha1();
        }
        else
        {
            // https://stackoverflow.com/a/64354296
            []<bool flag = false>() { static_assert(flag, "Bad Algorithm"); }();
        }
    };
    inline Algorithm getAlgorithm() const noexcept
    {
        return Alg;
    };
    HashTypeImpl hash(const HashTypeImpl &in) const
    {
        // хеш-значения
        HashTypeImpl value(static_cast<size_t>(EVP_MAX_MD_SIZE));
        // хеш-длина
        unsigned int len{0};
        // Освобождаем контекст по выходу
        struct EVP_MD_CTX_Deleter
        {
            void operator()(EVP_MD_CTX *context) const
            {
                EVP_MD_CTX_free(context);
            }
        };
        // контекст для хеширования
        std::unique_ptr<EVP_MD_CTX, EVP_MD_CTX_Deleter> context(EVP_MD_CTX_new());
        if (!context)
        {
            throw std::runtime_error("Failed to create EVP_MD_CTX");
        }

        // инициализация
        if (EVP_DigestInit_ex(context.get(), m_hash, NULL) != 1)
        {
            throw std::runtime_error("Failed to initialize digest");
        }

        // хеширование
        if (EVP_DigestUpdate(context.get(), in.data(), in.size()) != 1)
        {
            throw std::runtime_error("Failed to update digest");
        }

        // результат
        if (EVP_DigestFinal_ex(context.get(), value.data(), &len) != 1)
        {
            throw std::runtime_error("Failed to finalize digest");
        }
        value.resize(len);
        return value;
    };
};
template <> class HashImpl<HashTypeImpl, Algorithm::CRC32>
{
  public:
    inline Algorithm getAlgorithm() const noexcept
    {
        return Algorithm::CRC32;
    };
    HashTypeImpl hash(const HashTypeImpl &in) const
    {
        boost::crc_32_type crc;
        crc.process_bytes(in.data(), in.size());
        const size_t size = sizeof(value_type);
        union
        {
            unsigned int rez;
            value_type *buf;
        } val;
        val.buf = new value_type(size); // TODO: make sure, no memory leak !!!!
        val.rez = crc.checksum();
        return {val.buf, size};
    };
};

static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::SHA256>>,
              "Hasher does not satisfy Hasher concept");
static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::MD5>>, "Hasher does not satisfy Hasher concept");
static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::SHA1>>, "Hasher does not satisfy Hasher concept");
static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::CRC32>>, "Hasher does not satisfy Hasher concept");

} // namespace hash

#endif // HASH_H