#include <boost/crc.hpp>

#include "hash.h"

namespace hash {
    CRC32::CRC32() : HashImpl(hash::Algorithm::CRC32) {}

    HashType CRC32::hash(const HashType& in) {
        boost::crc_32_type crc;
        crc.process_bytes(in.c_str(), in.size());
        const size_t size = sizeof(unsigned int);
        union {
            unsigned int rez;
            char buf[size];
        } val;
        val.rez = crc.checksum();
        return {reinterpret_cast<const char*>(&val.buf), size};
    };
}  // namespace hash
