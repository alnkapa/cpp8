#ifndef FILE_FINDER_H
#define FILE_FINDER_H

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/key.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include "global/hash.h"
#include "hashing/hash.h"
#include <unordered_set>
#include "argc/argc.h"
#include "files/files.h"

using namespace boost::multi_index;
using DirectoryException = std::unordered_set<std::string>;

template <typename T>
struct file_extractor
{
    typedef std::string result_type;
    const result_type &operator()(const files::File<T> &e) const
    {
        return e.get_path();
    }
    result_type &operator()(files::File<T> *e) const
    {
        return e->path();
    }
};
template <typename T>
struct file_size_extractor
{
    typedef std::uintmax_t result_type;
    const result_type &operator()(const files::File<T> &e) const
    {
        return e.get_file_size();
    }
    result_type &operator()(files::File<T> *e) const
    {
        return e->file_size();
    }
};

inline std::size_t get_block_number(std::uintmax_t file_size, std::size_t block_size) noexcept
{
    auto block_number = file_size / block_size;
    if (file_size % block_size != 0)
    {
        block_number++;
    }
    return block_number;
}

template <typename T>
 requires hash::HashType<T>
struct proccess
{
    // clang-format off
    typedef multi_index_container<
        files::File<T>,
        indexed_by<hashed_unique<file_extractor<T>>, // поиск файла
        ordered_non_unique<file_size_extractor<T>>   // поиск размера
    >> container;
    // clang-format on                                         
    // хеш функция
    std::unique_ptr<hash::HasherBase<T>> h_fu;
    // параметры запуска
    arg::Argc arg;
    // просмотр файловой системы
    void file_finder(const std::string &path, std::shared_ptr<DirectoryException> directory_exception);
    // выбрать из индекса файлы одинаковые по размеру
    void file_cmp(const std::string &path, std::uintmax_t file_size, std::size_t block_size);
};

#endif