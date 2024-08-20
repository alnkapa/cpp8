#include "file_finder.h"
#include <filesystem>
// #include <boost/multi_index/hashed_index.hpp>
// #include <boost/multi_index/key.hpp>
// #include <boost/multi_index/ordered_index.hpp>
// #include <boost/multi_index_container.hpp>

// #include <fstream>
// #include <memory>
// #include <tuple>
// #include <unordered_set>
// #include <iostream>

template <typename T>
void proccess<T>::file_finder(const std::string &path, std::shared_ptr<DirectoryException> directory_exception)
{
    namespace fs = std::filesystem;
    fs::path dir(path);
    for (auto dir_it = fs::recursive_directory_iterator(dir); dir_it != fs::recursive_directory_iterator(); ++dir_it)
    {
        if (dir_it->is_directory())
        {
            if (dir_it.depth() + 1 > arg.deep_level)
            {
                dir_it.disable_recursion_pending();
            }
            else if (directory_exception->contains(dir_it->path().filename()))
            {
                dir_it.disable_recursion_pending();
            }
        }
        else if (dir_it->is_regular_file() && dir_it->file_size() >= arg.file_size)
        {
            auto block_number = get_block_number(dir_it->file_size(), arg.block_size);
            file_cmp(dir_it->path(), dir_it->file_size(), block_number);
        }
    }
}

template <typename T>
void proccess<T>::file_cmp(const std::string &path, std::uintmax_t file_size, std::size_t block_size)
{
    auto &file_size_index = container.get<1>();
    auto file_size_range_it = file_size_index.equal_range(file_size);
    if (file_size_range_it.first == file_size_range_it.second)
    {
        // совпадений по размеру не найдено
        return;
    }

}
