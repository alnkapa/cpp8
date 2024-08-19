#ifndef HASH_TYPE_H
#define HASH_TYPE_H
#include <openssl/evp.h>

#include <memory>
#include <vector>

// #include "./global/hash.h"

namespace hash
{

class HashTypeImpl final : public HashType
{
  private:
    std::vector<HashType::value_type> m_buf;

  public:
    HashTypeImpl(size_t len = 0);
    HashTypeImpl(const HashType &);
    HashTypeImpl &operator=(const HashType &);
    HashTypeImpl(HashType &&);
    HashTypeImpl &operator=(HashType &&);
    virtual ~HashTypeImpl() {};
    bool operator==(const HashType &) noexcept override;
    size_t size() const noexcept override;
    void resize(size_t len) const noexcept;
    HashType::value_type *data() override;
    const HashType::value_type *data() const override;
};
} // namespace hash

#endif