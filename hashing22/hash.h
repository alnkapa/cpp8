#ifndef HASH_H
#define HASH_H
#include <openssl/evp.h>
#include <memory>
#include "../global/hash.h"
#include "hash_type.h"

namespace hash
{

class HashImpl : public Hash
{
  private:
    // хеш-функция
    const EVP_MD *m_hash;
    // хеш-алгоритм
    Algorithm m_algorithm;
    HashTypeImpl hash(const HashType &in);

  protected:
    explicit HashImpl(Algorithm);
    explicit HashImpl(const EVP_MD *, Algorithm);

  public:
    virtual ~HashImpl();
    static std::unique_ptr<HashImpl> Create(Algorithm);
    // Метод для хеширования данных
    virtual std::unique_ptr<HashType> hash(const HashType &in) const override;
    // Метод для получения алгоритма
    Algorithm getAlgorithm() const noexcept final override;
};

class SHA256 final : public HashImpl
{
  public:
    SHA256();
};
class MD5 final : public HashImpl
{
  public:
    MD5();
};
class SHA1 final : public HashImpl
{
  public:
    SHA1();
};
class CRC32 final : public HashImpl
{
  private:
    HashTypeImpl hash(const HashType &in);

  public:
    CRC32();
    virtual std::unique_ptr<HashType> hash(const HashType &in) const override;
};
} // namespace hash

#endif // HASH_H