#ifndef BLOCK_H
#define BLOCK_H
#include <memory>
#include <optional>
#include <vector>

#include "../global/hash.h"

namespace files {
    using BlockVector = std::vector<std::optional<::hash::HashType>>;
    class File {
       private:
        std::string m_path{};                         // путь до файла
        BlockVector m_blocks{};                          // массив блоков в файле
        std::uintmax_t m_file_size{0};                // размер файла
        std::size_t m_block_size{0};                  // размер блока
        std::size_t m_block_numbers{0};               //  количество блоков
        hash::HashType m_hash_value{0};               // хеш
        std::weak_ptr<::hash::Hash> m_hash_function;  // функция хеширования
        // содержимое блока
        ::hash::HashType get_block_contain(const size_t block_index);
        // получить хеш
        void get_block_hash(const ::hash::HashType& in);

       public:
        explicit File(const std::string& path, const std::uintmax_t& file_size, const std::size_t& block_size,
                      const std::size_t& block_numbers, const std::weak_ptr<::hash::Hash>& hash);
        explicit File(const std::string& path, const std::uintmax_t& file_size, const std::size_t& block_size,
                      const std::size_t& block_numbers, const std::weak_ptr<::hash::Hash>& hash, BlockVector&& blocks);
        // размер файла
        const std::uintmax_t& get_file_size() const noexcept;
        // размер файла
        std::uintmax_t& file_size() noexcept;
        // путь до файла
        const std::string& get_path() const noexcept;
        // путь до файла
        std::string& path() noexcept;
    };

}  // namespace files
#endif  // BLOCK_H