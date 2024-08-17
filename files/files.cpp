#include "files.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace files
{
File::File(const std::string &path,
           const std::uintmax_t &file_size,
           const std::size_t &block_size,
           const std::size_t &block_numbers)
    : m_path(path), m_file_size(file_size), m_block_size(block_size), m_block_numbers(block_numbers)
{    
}


const std::uintmax_t &File::get_file_size() const noexcept
{
    return m_file_size;
}

std::uintmax_t &File::file_size() noexcept
{
    return m_file_size;
}

const std::string &File::get_path() const noexcept
{
    return m_path;
}

std::string &File::path() noexcept
{
    return m_path;
}

} // namespace files