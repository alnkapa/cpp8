#include "files.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace files
{
// File::File(const std::string &path,
//            const std::uintmax_t &file_size,
//            const std::size_t &block_size,
//            const std::size_t &block_numbers)
//     : m_path(path), m_file_size(file_size), m_block_size(block_size), m_block_numbers(block_numbers)
// {
//     m_blocks.reserve(m_block_size);
// }

// const std::uintmax_t &File::get_file_size() const noexcept
// {
//     return m_file_size;
// }

// std::uintmax_t &File::file_size() noexcept
// {
//     return m_file_size;
// }

// const std::string &File::get_path() const noexcept
// {
//     return m_path;
// }

// std::string &File::path() noexcept
// {
//     return m_path;
// }

// const Block_t &File::get_block(std::size_t num) const noexcept
// {
//     return m_blocks[num];
// }

// Block_t &File::block(std::size_t num) noexcept
// {
//     return m_blocks[num];
// }

// bool File::is_block(std::size_t num) const noexcept
// {
//     try
//     {
//         m_blocks.at(num);
//         return true;
//     }
//     catch (const std::out_of_range &ex)
//     {
//     }
//     return false;
// }

// void File::add_block(Block_t &block, std::size_t num) noexcept
// {
//     m_blocks.assign(num, block);
// }

} // namespace files