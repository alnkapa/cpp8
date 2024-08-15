#include "block.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

::hash::HashType Block::get_block_contain() {
    auto file = std::fstream(m_path, std::ios::in | std::ios::binary);
    char buf[m_block_size]{};
    file.seekg(m_block_index * m_block_size);
    std::cout << file.tellg() << " " << m_block_index * m_block_size << " : ";
    auto size = file.read(reinterpret_cast<char*>(&buf), m_block_size).gcount();
    if (size == 0 || size > m_block_size) {
        throw(std::length_error("block size mismatch: " + size));
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
             const std::size_t& block_size, const std::weak_ptr<::hash::Hash>& hash)
    : m_path(path), m_block_index{index}, m_file_size(file_size), m_block_size(block_size), m_hash_function(hash) {}

const hash::HashType& Block::get_hash() {
    if (m_hash_value.empty()) {
        get_block_hash(get_block_contain());
    }
    return m_hash_value;
}

const std::size_t& Block::get_index() const noexcept { return m_block_index; }

std::size_t& Block::index() noexcept { return m_block_index; }

const std::uintmax_t& Block::get_file_size() const noexcept { return m_file_size; }

std::uintmax_t& Block::file_size() noexcept { return m_file_size; }

const std::string& Block::get_path() const noexcept { return m_path; }

std::string& Block::path() noexcept { return m_path; };

bool operator!=(Block& lhs, Block& rhs) { return !(lhs == rhs); }

bool operator==(Block& lhs, Block& rhs) {
    if (lhs.m_block_index != rhs.m_block_index) {
        return false;
    }
    if (lhs.m_path != rhs.m_path) {
        return false;
    }
    return lhs.get_hash() != rhs.get_hash();
}
