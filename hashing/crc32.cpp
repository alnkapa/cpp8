// #include <boost/crc.hpp>

// #include "hash.h"

// namespace hash
// {
// HashTypeImpl CRC32::hash(const HashType &in)
// {
//     boost::crc_32_type crc;
//     crc.process_bytes(in.data(), in.size());
//     const size_t size = sizeof(unsigned int);
//     HashTypeImpl vec(size);

//     union
//     {
//         unsigned int rez;
//         unsigned char *buf;
//     } val;

//     val.buf = vec.data();
//     val.rez = crc.checksum();
//     return vec;
// }

// CRC32::CRC32() : HashImpl(hash::Algorithm::CRC32)
// {
// }
// } // namespace hash
