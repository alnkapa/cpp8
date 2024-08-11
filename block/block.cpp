#include "block.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

// закрыть файл
void close_file(std::fstream* ptr) {
    if (ptr->is_open()) ptr->close();
};

std::size_t Block::get_block_number() noexcept {
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
    if (size == 0 || size > m_block_size) {
        throw(std::ios_base::failure("block size mismatch: " + size));
    }
    std::cout << size << " ";
    return {reinterpret_cast<const char*>(&buf), std::size_t(size)};
}

void Block::get_block_hash(const ::hash::HashType& in) {
    if (auto ptr = m_hash_function.lock()) {
        m_hash_value = std::move(ptr->hash(in));
    } else {
        throw(std::invalid_argument("iil hash function"));
    }
}

Block::Block(const std::string& path, const std::size_t& index, const std::uintmax_t& file_size,
             const std::size_t& block_size, std::shared_ptr<::hash::Hash> hash)
    : m_path(path), m_block_index{index}, m_file_size(file_size), m_block_size(block_size), m_hash_function(hash) {
    m_block_number = get_block_number();
}

const hash::HashType& Block::get_hash() {
    if (m_hash_value.empty()) {
        get_block_hash(get_block_contain());
    }
    return m_hash_value;
}

std::size_t Block::get_index() const noexcept { return m_block_index; }

const std::string& Block::get_path() const noexcept { return m_path; };
