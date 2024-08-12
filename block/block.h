#ifndef BLOCK_H
#define BLOCK_H
#include <boost/multi_index_container.hpp>
#include <memory>

#include "../global/hash.h"

using namespace boost::multi_index;

class Block {
   private:
    std::string m_path{};                         // путь до файла
    std::size_t m_block_index{};                  // номер блока в файле
    std::uintmax_t m_file_size{};                 // размер файла
    std::size_t m_block_size{};                   // размер блока
    hash::HashType m_hash_value{};                // хеш
    std::weak_ptr<::hash::Hash> m_hash_function;  // функция хеширования    
    // содержимое блока
    ::hash::HashType get_block_contain();
    // получить хеш
    void get_block_hash(const ::hash::HashType& in);

   public:
    Block(const std::string& path, const std::size_t& index, const std::uintmax_t& file_size,
          const std::size_t& block_size, std::shared_ptr<::hash::Hash> hash);
    // хеш блока
    const hash::HashType& get_hash();
    // номер блока
    std::size_t get_index() const noexcept;
    // размер файла 
    std::uintmax_t get_file_size() const noexcept;
    // путь до файла
    const std::string& get_path() const noexcept;
};

#endif  // BLOCK_H