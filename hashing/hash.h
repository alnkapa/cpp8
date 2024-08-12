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
        HashImpl(const EVP_MD* hash, hash::Algorithm algorithm);

       public:
        HashImpl(const HashImpl&) = delete;
        HashImpl(const HashImpl&&) = delete;
        HashImpl& operator=(const HashImpl&) = delete;
        HashImpl& operator=(HashImpl&&) = delete;
        virtual ~HashImpl();
        static std::unique_ptr<HashImpl> Create(hash::Algorithm);
        // Метод для хеширования данных
        HashType hash(const HashType& in);  // TODO: final
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
}  // namespace hash

#endif  // HASH_H