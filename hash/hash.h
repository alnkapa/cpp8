#pragma once
#include <stdint.h>

namespace hash {
    enum class Type {
        crc32,
        md5,
    };
    template <Type type, std::size_t N>
    class Hash {
        char[N] hash()
    };
};  // namespace hash
