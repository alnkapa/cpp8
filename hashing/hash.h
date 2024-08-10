#ifndef HASH_H
#define HASH_H
#include <openssl/evp.h>

#include <memory>
#include <string>

// Перечисление для алгоритмов хеширования
enum class HashAlgorithm { SHA256, MD5, SHA1 };
// тип данных для хранения принимаемого и передаваемого значения
using HashType = std::string;

class Hash {
   private:    
    // хеш-функция
    const EVP_MD* m_hash;
    // хеш-алгоритм
    HashAlgorithm m_algorithm;

   protected:
    Hash(const EVP_MD* hash, HashAlgorithm algorithm);

   public:
    Hash(const Hash&) = delete;
    Hash(const Hash&&) = delete;
    Hash& operator=(const Hash&) = delete;
    Hash& operator=(Hash&&) = delete;
    ~Hash() = default;
    static std::unique_ptr<Hash> createHash(HashAlgorithm);
    // Метод для хеширования данных
    HashType hash(const HashType& in);
    // Метод для получения алгоритма
    HashAlgorithm getAlgorithm() const;    
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
#endif  // HASH_H