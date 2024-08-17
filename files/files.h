#ifndef BLOCK_H
#define BLOCK_H
#include <memory>
#include <optional>
#include <vector>

#include "../global/hash.h"

namespace files
{
using Block_t = ::hash::HashType;
using BlockVector = std::vector<Block_t>;
class File
{
  private:
    std::string m_path{};           // путь до файла
    std::uintmax_t m_file_size{0};  // размер файла
    std::size_t m_block_size{0};    // размер блока
    std::size_t m_block_numbers{0}; //  количество блоков

  public:
    BlockVector blocks{}; // массив блоков в файле
    explicit File(const std::string &path,
                  const std::uintmax_t &file_size,
                  const std::size_t &block_size,
                  const std::size_t &block_numbers);
    // размер файла
    const std::uintmax_t &get_file_size() const noexcept;
    // размер файла
    std::uintmax_t &file_size() noexcept;
    // путь до файла
    const std::string &get_path() const noexcept;
    // путь до файла
    std::string &path() noexcept;
};

} // namespace files
#endif // BLOCK_H