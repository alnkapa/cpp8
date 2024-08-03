#pragma once
#include <string>
#include <vector>
#include <iostream>

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
    int parse(int argc, char* argv[]);
};

std::istream& operator>> (std::istream &, Argc::Hashes &);