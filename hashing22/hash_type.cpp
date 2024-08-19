#include "hash_type.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace hash
{

HashTypeImpl::HashTypeImpl(size_t len)
{
    m_buf.reserve(len);
}

HashTypeImpl::HashTypeImpl(const HashType &other)
{
    if (this != &other)
    {
        m_buf.resize(other.size());
        std::copy(other.m_buf.begin(), other.m_buf.end(), m_buf.begin());
    }
}

HashTypeImpl &HashTypeImpl::operator=(const HashType &other)
{
    if (this != &other)
    {
    }
    return *this;
}

HashTypeImpl::HashTypeImpl(HashType &&other)
{
    if (this != &other)
    {
    }
}

HashTypeImpl &HashTypeImpl::operator=(HashType &&other)
{
    if (this != &other)
    {
    }
    return *this;
}

// std::ostream &HashTypeImpl::operator<<(std::ostream &stream)
// {
//     for (const auto &v : m_buf)
//     {
//         stream << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (0xFF & v);
//     };
//     return stream;
// }

bool HashTypeImpl::operator==(const HashType &) noexcept
{
    return false;
}

size_t HashTypeImpl::size() const noexcept
{
    return size_t();
}

void HashTypeImpl::resize(size_t len) const noexcept
{
}

HashType::value_type *HashTypeImpl::data()
{
    // TODO: insert return statement here
    return nullptr;
}

const HashType::value_type *HashTypeImpl::data() const
{
    return nullptr;
}

} // namespace hash