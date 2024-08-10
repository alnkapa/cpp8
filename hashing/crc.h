#include <boost/crc.hpp>
#include <iostream>

int main() {
    boost::crc_32_type crc;

    const char* data = "Hello, World!";
    crc.process_bytes(data, strlen(data));
    std::cout << "CRC32: " << std::hex << crc.checksum() << std::endl;
};
