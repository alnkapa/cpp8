#ifndef BLOCK_H
#define BLOCK_H
#include "./global/hash.h"
#include <boost/multi_index_container.hpp>

using namespace boost::multi_index;

class Block {
   private:
    std::string m_path;          // путь до файла
    std::size_t m_index;         // номер блока в файле
    std::uintmax_t m_file_size;  // размер файла
                                 //    T m_hash;                    // хеш
   public:
    // Block(const std::string& path, const std::size_t& index, std::uintmax_t file_size, const T& hash)
    //     : m_path(path), m_index(index), m_file_size(file_size), m_hash(hash){};
    // std::size_t index() { return m_index; };
    // // количество блоков
    // std::size_t get_block_number(std::uintmax_t file_size, std::size_t block_size) {
    //     auto block_number = file_size / block_size;
    //     if (file_size % block_size != 0) {
    //         block_number++;
    //     }
    //     return block_number;
    // }

    // // содержимое блока
    // std::size_t get_block_hash(const std::string& path, std::uintmax_t file_size, std::size_t block_index,
    //                            std::size_t block_size) {
    //     std::unique_ptr<std::fstream, decltype(&close_file)> file_ptr(
    //         new std::fstream(path, std::ios::in | std::ios::binary), &close_file);
    //     if (!file_ptr->is_open()) {
    //         throw(std::ios_base::failure("can`t open file: " + path));
    //     }
    //     auto h = hash::Hash::Create(hash::Algorithm::MD5);
    //     char buf[block_size]{};
    //     file_ptr->seekg(block_index * block_size);
    //     std::cout << file_ptr->tellg() << " " << block_index * block_size << " : ";
    //     auto size = file_ptr->read(reinterpret_cast<char*>(&buf), block_size).gcount();
    //     std::cout << size << " ";
    //     h->hash(buf);
    //     // Do hash
    //     return {0};
    // }
    // T hash() {
    //     if (m_hash.size() == 0) {
    //         std::fstream my_file(m_path);
    //         if (!my_file) {
    //             throw(std::filesystem::filesystem_error("can`t open file", std::filesystem::path(m_path)));
    //         }
    //         return m_hash;
    //     } else {
    //         return m_hash;
    //     }
    // };
};

#endif  // BLOCK_H