#pragma once
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace arg {
    struct Argc {
        enum class Hashes {
            crc32,
            md5,
        };
        std::vector<std::string> directory;  // директории для сканирования
        std::vector<std::string> directory_exception;  // директории для исключения из сканирования
        int deep_level;                                // уровень сканирования
        std::size_t file_size;                         // минимальный размер файла
        std::string wildcards;                         // маски имен файлов
        std::size_t block_size;                        // размер блока
        Hashes hashes;                                 // алгоритмов хэширования
        int parse(int argc, char *argv[]);
    };
};  // namespace arg

namespace boost {
    template <>
    inline arg::Argc::Hashes lexical_cast<arg::Argc::Hashes>(const std::string &arg) {
        if (arg == "crc32") {
            return arg::Argc::Hashes::crc32;
        } else if (arg == "md5") {
            return arg::Argc::Hashes::md5;
        } else {
            throw std::invalid_argument("Invalid hash algorithm. Allowed values are: crc32, md5");
        }
    };
    template <>
    inline std::string lexical_cast<std::string>(const arg::Argc::Hashes &arg) {
        switch (arg) {
            case arg::Argc::Hashes::crc32:
                return "crc32";
            case arg::Argc::Hashes::md5:
                return "md5";
            default:
                throw std::invalid_argument("Invalid hash algorithm");
        };
    };
}  // namespace boost