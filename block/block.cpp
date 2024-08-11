#include "block.h"

#include <fstream>
#include <iostream>
#include <utility>

// закрыть файл
void close_file(std::fstream* ptr) {
    if (ptr->is_open()) ptr->close();
};

std::size_t Block::get_block_number() {
    auto block_number = m_file_size / m_block_size;
    if (m_file_size % m_block_size != 0) {
        block_number++;
    }
    return block_number;
}

::hash::HashType Block::get_block_contain() {
    std::unique_ptr<std::fstream, decltype(&close_file)> file_ptr(
        new std::fstream(m_path, std::ios::in | std::ios::binary), &close_file);
    if (!file_ptr->is_open()) {
        throw(std::ios_base::failure("can`t open file: " + m_path));
    }
    char buf[m_block_size]{};
    file_ptr->seekg(m_block_index * m_block_size);
    std::cout << file_ptr->tellg() << " " << m_block_index * m_block_size << " : ";
    auto size = file_ptr->read(reinterpret_cast<char*>(&buf), m_block_size).gcount();
    if (!size) {
        throw(std::ios_base::failure("block size: " + size));
    }
    std::cout << size << " ";
    return {reinterpret_cast<const char*>(&buf), std::size_t(size)};
}

void Block::get_block_hash(const ::hash::HashType& in) { m_hash_value = std::move(m_hash_function.get()->hash(in)); }

Block::Block(const std::string& path, const std::size_t& index, const std::uintmax_t& file_size,
             const std::size_t& block_size, std::unique_ptr<::hash::Hash> hash)
    : m_path(path), m_block_index{index}, m_file_size(file_size), m_block_size(block_size), m_hash_function(std::move(hash)) {
    m_block_number = get_block_number();
};
