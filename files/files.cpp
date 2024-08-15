#include "files.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace files {
    ::hash::HashType File::get_block_contain(const size_t block_index) {
        auto file = std::fstream(m_path, std::ios::in | std::ios::binary);
        char buf[m_block_size]{};
        file.seekg(block_index * m_block_size);
        std::cout << file.tellg() << " " << block_index * m_block_size << " : ";
        auto size = file.read(reinterpret_cast<char*>(&buf), m_block_size).gcount();
        if (size == 0 || size > m_block_size) {
            throw(std::length_error("block size mismatch: " + size));
        }
        std::cout << size << " ";
        return {reinterpret_cast<const char*>(&buf), std::size_t(size)};
    }

    void File::get_block_hash(const ::hash::HashType& in) {
        if (auto ptr = m_hash_function.lock()) {
            m_hash_value = std::move(ptr->hash(in));
        } else {
            throw(std::invalid_argument("iil hash function"));
        }
    }

    File::File(const std::string& path, const std::uintmax_t& file_size, const std::size_t& block_size,
               const std::size_t& block_numbers, const std::weak_ptr<::hash::Hash>& hash)
        : m_path(path),
          m_file_size(file_size),
          m_block_size(block_size),
          m_block_numbers(block_numbers),
          m_hash_function(hash) {
        // m_blocks.reserve(block_numbers);
    }

    File::File(const std::string& path, const std::uintmax_t& file_size, const std::size_t& block_size,
               const std::size_t& block_numbers, const std::weak_ptr<::hash::Hash>& hash, BlockVector&& blocks)
        : m_path(path),
          m_file_size(file_size),
          m_block_size(block_size),
          m_block_numbers(block_numbers),
          m_hash_function(hash),
          m_blocks{std::move(blocks)} {}

    // const hash::HashType& File::get_hash() {
    //     if (m_hash_value.empty()) {
    //         get_block_hash(get_block_contain());
    //     }
    //     return m_hash_value;
    // }

    const std::uintmax_t& File::get_file_size() const noexcept { return m_file_size; }

    std::uintmax_t& File::file_size() noexcept { return m_file_size; }

    const std::string& File::get_path() const noexcept { return m_path; }

    std::string& File::path() noexcept { return m_path; };

}  // namespace files