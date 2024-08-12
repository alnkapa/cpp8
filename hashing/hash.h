#ifndef HASH_H
#define HASH_H
#include <openssl/evp.h>

#include <memory>
#include <string>

#include "./global/hash.h"

namespace hash {

    class HashImpl : public Hash {
       private:
        // хеш-функция
        const EVP_MD* m_hash;
        // хеш-алгоритм
        hash::Algorithm m_algorithm;

       protected:
        explicit HashImpl(hash::Algorithm algorithm);
        explicit HashImpl(const EVP_MD* hash, hash::Algorithm algorithm);

       public:
        virtual ~HashImpl();
        static std::unique_ptr<HashImpl> Create(hash::Algorithm);
        // Метод для хеширования данных
        virtual HashType hash(const HashType& in);  // TODO: final
        // Метод для получения алгоритма
        hash::Algorithm getAlgorithm() const noexcept final;
    };

    class SHA256 final : public HashImpl {
       public:
        SHA256();
    };
    class MD5 final : public HashImpl {
       public:
        MD5();
    };
    class SHA1 final : public HashImpl {
       public:
        SHA1();
    };
    class CRC32 final : public HashImpl {
       public:
        CRC32();
        HashType hash(const HashType& in);
    };
}  // namespace hash

#endif  // HASH_H