#ifndef BLOCK_H
#define BLOCK_H
#include <memory>
#include <optional>
#include <vector>

#include "../hashing/hash_type.h"

namespace files
{

class File
{
  public:
    std::uintmax_t size{0}; // размер файла
    std::string path{};     // путь до файла
  private:
    std::vector<hash::HashTypeImpl> m_blocks{}; // массив блоков в файле
  public:
    explicit File(const std::string &path, const std::uintmax_t &size, std::vector<hash::HashTypeImpl> &&blocks)
        : path(path), size(size), m_blocks(std::move(blocks)) {};

    // получить хеш
    const hash::HashTypeImpl &operator[](std::size_t num) const noexcept
    {
        return m_blocks[num];
    };
    // есть ли хеш
    bool is_block(std::size_t num) const noexcept
    {
        try
        {
            m_blocks.at(num);
            return true;
        }
        catch (const std::out_of_range &ex)
        {
        }
        return false;
    };
    // добавить хеш
    void assign(std::size_t num, const hash::HashTypeImpl &block) noexcept {

    };
};

} // namespace files
#endif // BLOCK_H