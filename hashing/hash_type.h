#ifndef HASH_TYPE_H
#define HASH_TYPE_H
#include "../global/hash.h"
#include <vector>

namespace hash
{

class HashTypeImpl : private std::vector<value_type>
{
  public:
    using std::vector<value_type>::size;
    using std::vector<value_type>::data;
    HashTypeImpl(const value_type *ptr, size_type size) : std::vector<value_type>(ptr, ptr + size) {};
};

static_assert(hash::HashType<HashTypeImpl>, "HashTypeImpl does not satisfy HashType concept");

} // namespace hash

#endif