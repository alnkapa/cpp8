#ifndef BLOCK_H
#define BLOCK_H
#include <memory>
#include <optional>
#include <vector>

#include "../global/hash.h"

namespace files
{

template <typename T>
    requires hash::HashType<T>
class File
{
  public:
    using block_type = T;
    using vector_type = std::vector<block_type>;

  private:
    std::string m_path{};           // путь до файла
    std::uintmax_t m_file_size{0};  // размер файла
    std::size_t m_block_size{0};    // размер блока
    std::size_t m_block_numbers{0}; //  количество блоков
    vector_type m_blocks{};         // массив блоков в файле
  public:
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

    // получить хеш
    const block_type &get_block(std::size_t num) const noexcept;
    // получить хеш
    block_type &block(std::size_t num) noexcept;
    // есть ли хеш
    bool is_block(std::size_t num) const noexcept;
    // добавить хеш
    void add_block(block_type &block, std::size_t num) noexcept;
};

} // namespace files
#endif // BLOCK_H