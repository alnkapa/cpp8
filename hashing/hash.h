#ifndef HASH_H
#define HASH_H
#include <openssl/evp.h>

#include <memory>
#include <string>

#include "./global/hash.h"

namespace hash {

    class Hash {
       private:
        // хеш-функция
        const EVP_MD* m_hash;
        // хеш-алгоритм
        hash::Algorithm m_algorithm;

       protected:
        Hash(const EVP_MD* hash, hash::Algorithm algorithm);

       public:
        Hash(const Hash&) = delete;
        Hash(const Hash&&) = delete;
        Hash& operator=(const Hash&) = delete;
        Hash& operator=(Hash&&) = delete;
        ~Hash() = default;
        static std::unique_ptr<Hash> Create(hash::Algorithm);
        // Метод для хеширования данных
        HashType hash(const HashType& in);
        // Метод для получения алгоритма
        hash::Algorithm getAlgorithm() const;
    };

    class SHA256 : public Hash {
       public:
        SHA256();
    };
    class MD5 : public Hash {
       public:
        MD5();
    };
    class SHA1 : public Hash {
       public:
        SHA1();
    };
}  // namespace hash

#endif  // HASH_H