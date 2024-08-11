#ifndef BLOCK_H
#define BLOCK_H
#include <boost/multi_index_container.hpp>
#include <memory>

#include "../global/hash.h"

using namespace boost::multi_index;

class Block {
   private:
    std::string m_path{};           // путь до файла
    std::size_t m_block_index{};    // номер блока в файле
    std::uintmax_t m_file_size{};   // размер файла
    std::size_t m_block_size{};     // размер блока
    hash::HashType m_hash_value{};  // хеш
    std::size_t m_block_number{};   // количество блоков
    std::unique_ptr<::hash::Hash> m_hash;
    // количество блоков
    std::size_t get_block_number();
    // содержимое блока
    ::hash::HashType get_block_contain();
    // получить хеш
    void get_block_hash(const ::hash::HashType& in);

   public:
    Block(const std::string& path, const std::size_t& index, const std::uintmax_t& file_size,
          const std::size_t& block_size, std::unique_ptr<::hash::Hash> hash);
};

#endif  // BLOCK_H