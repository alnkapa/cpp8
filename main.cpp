#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/key.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <tuple>
#include <unordered_set>
#include <iostream>
// #include "argc/argc.h"
// #include "files/files.h"
#include "global/hash.h"
// #include "hashing/hash.h"

using namespace boost::multi_index;
using DirectoryException = std::unordered_set<std::string>;
// using namespace hash;

// struct BlockSave
// {
//     BlockSave(const HashType &value, const size_t index) : m_value(value), m_index(index)
//     {
//     }

//     void operator()(files::File &e)
//     {
//         std::cout << "operator: " << m_index << "\n";
//         e.add_block(m_value, m_index);
//     }

//   private:
//     size_t m_index;
//     HashType m_value;
// };

// struct file_extractor
// {
//     typedef std::string result_type;
//     const result_type &operator()(const files::File &e) const
//     {
//         return e.get_path();
//     }
//     result_type &operator()(files::File *e) const
//     {
//         return e->path();
//     }
// };

// struct file_size_extractor
// {
//     typedef std::uintmax_t result_type;
//     const result_type &operator()(const files::File &e) const
//     {
//         return e.get_file_size();
//     }
//     // result_type &operator()(files::File *e) const
//     // {
//     //     return e->file_size();
//     // }
// };
// // clang-format off
// typedef multi_index_container<
//   files::File,
//   indexed_by<
//     hashed_unique<file_extractor>, // поиск файла
//     ordered_non_unique<file_size_extractor> // поиск размера
//   >
// > container;

// // clang-format on
// struct proccess
// {
//     std::shared_ptr<Hash> hash_function;
//     arg::Argc arg;
//     container index;

//     std::size_t get_block_number(std::uintmax_t file_size, std::size_t block_size) noexcept
//     {
//         auto block_number = file_size / block_size;
//         if (file_size % block_size != 0)
//         {
//             block_number++;
//         }
//         return block_number;
//     }
//     HashTypeImpl get_block_hash(const std::string &path, const size_t block_index, const size_t block_size)
//     {
//         auto file = std::fstream(path, std::ios::in | std::ios::binary);
//         if (!file.is_open())
//         {
//             throw std::runtime_error("Failed to open file: " + path);
//         }
//         HashTypeImpl buf(block_size);
//         file.seekg(block_index * block_size);
//         std::cout << file.tellg() << " " << block_index * block_size << " : ";
//         auto size = file.read(reinterpret_cast<char *>(buf.data()), block_size).gcount();
//         if (size == 0 || size > block_size)
//         {
//             throw std::length_error("block size mismatch: " + std::to_string(size));
//         }
//         std::cout << size << " ";
//         return hash_function->hash(buf);
//     }

//     void add_to_index(const std::string &file_name, std::uintmax_t file_size, std::size_t block_numbers)
//     {
//         std::cout << "IN: File: " << file_name << ", Size: " << file_size << "\n";
//         index.emplace(file_name, file_size, arg.block_size, block_numbers);
//     }

//     // просмотр файловой системы
//     void file_finder(const std::string &path, std::shared_ptr<DirectoryException> directory_exception)
//     {
//         namespace fs = std::filesystem;
//         fs::path dir(path);
//         auto &file_index = index.get<0>();
//         auto &file_size_index = index.get<1>();
//         for (auto dir_it = fs::recursive_directory_iterator(dir); dir_it != fs::recursive_directory_iterator();
//              ++dir_it)
//         {
//             if (dir_it->is_directory())
//             {
//                 if (dir_it.depth() + 1 > arg.deep_level)
//                 {
//                     dir_it.disable_recursion_pending();
//                 }
//                 else if (directory_exception->contains(dir_it->path().filename()))
//                 {
//                     dir_it.disable_recursion_pending();
//                 }
//             }
//             else if (dir_it->is_regular_file() && dir_it->file_size() >= arg.file_size)
//             {
//                 auto block_number = get_block_number(dir_it->file_size(), arg.block_size);
//                 if (index.empty())
//                 {
//                     // индекс пустой
//                     add_to_index(dir_it->path(), dir_it->file_size(), block_number);
//                     continue;
//                 }
//                 if (auto file_name_it = file_index.find(dir_it->path()); file_name_it != file_index.end())
//                 {
//                     // файл уже проиндексирован
//                     continue;
//                 }
//                 auto file_size_range_it = file_size_index.equal_range(dir_it->file_size());
//                 if (file_size_range_it.first == file_size_range_it.second)
//                 {
//                     // совпадений по размеру не найдено
//                     add_to_index(dir_it->path(), dir_it->file_size(), block_number);
//                     continue;
//                 }
//                 std::cout << "COMP: File: " << dir_it->path() << ", Size: " << dir_it->file_size() << "\n";
//                 files::BlockVector blocks{block_number};
//                 // std::unordered_set<container::iterator> skipped;
//                 //  по блочное сравнение
//                 // for (std::size_t b_num = 0; b_num < block_number; ++b_num)
//                 // {
//                     //std::cout << "BLOCK: " << b_num << "\n";
//                     // читаем очередной блок
//                     // auto c_bb = blocks[b_num] = get_block_hash(dir_it->path(), b_num, arg.block_size);
//                     // // по каждому файлу, который совпадает по размеру
//                     // for (auto &file_size_it = file_size_range_it.first; file_size_it != file_size_range_it.second;
//                     //      ++file_size_it)
//                     // {
//                     //     std::cout << "FIND: File: " << file_size_it->get_path() << "\n";
//                     //     auto file_index_it = index.project<0>(file_size_it); // Переход к file_index
//                     //     HashType f_bb;
//                     //     if (!file_index_it->is_block(b_num))
//                     //     {
//                     //         // загрузить очередной кусок в файл
//                     //         f_bb = get_block_hash(file_size_it->get_path(), b_num, arg.block_size);
//                     //         file_index.modify(file_index_it, BlockSave(f_bb, b_num));
//                     //     }
//                     //     else
//                     //     {
//                     //         f_bb = file_index_it->get_block(b_num);
//                     //     }
//                     //     if (f_bb != c_bb)
//                     //     { // сравнение блоков
//                     //       // сохранить все ранее прочитанные блоки для этого файла
//                     //       // исключить его их дальнейшего сравнения
//                     //       // skipped.emplace(file_size_it);
//                     //       // std::cout << "f_bb: " << f_bb << "\n";
//                     //       // std::cout << "c_bb: " << c_bb << "\n";
//                     //       // std::cout << "Address: " << &(*file_size_it) << std::endl;
//                     //       // file_size_it = file_size_range_it.erase(file_size_it);
//                     //     }
//                     //     else
//                     //     {
//                     //         //std::cout << "f_bb: " << f_bb << "\n";
//                     //         //std::cout << "c_bb: " << c_bb << "\n";
//                     //     }
//                     }
//                     // if (b_num == 2)
//                     // {
//                     //     std::terminate();
//                     // }
//                 }
//             }
//         };
//     }
// };

// Класс, представляющий данные
class MyData
{
  public:
    MyData(const std::string &data) : data_(data)
    {
    }

    MyData(const unsigned char *, std::size_t)  
    {
    }

    std::size_t size() const
    {
        return data_.size();
    }

    const unsigned char *data() const
    {
        return reinterpret_cast<const unsigned char *>(data_.data());
    }

  private:
    std::string data_;
};

// Класс, представляющий данные
class MyData1
{
  public:
    MyData1(const std::string &data) : data_(data)
    {
    }

    // std::size_t size() const
    // {
    //     return data_.size();
    // }

    const unsigned char *data() const
    {
        return reinterpret_cast<const unsigned char *>(data_.data());
    }

  private:
    std::string data_;
};

template <typename T>
    requires hash::HashType<T>
class MyHasher
{
  public:
    T hash(const T &data)
    {
        data.size();
        data.data();
        size_t block_size{10};
        unsigned char buf[block_size]{0};

        return T{&buf, block_size};
    }

    hash::Algorithm getAlgorithm() const
    {
        return hash::Algorithm::SHA256;
    }
};

template <typename T>
    requires hash::HashType<T>
class MyHasher1
{
  public:
    T hash(const T &data)
    {
        data.size();
        data.data();
        return data;
    }

    // hash::Algorithm getAlgorithm() const
    // {
    //     return hash::Algorithm::SHA256;
    // }
};

// Шаблонная функция, использующая концепты
template <typename T, typename H>
    requires hash::HashType<T> && hash::Hasher<T, H>
void processHash(T data, H hasher)
{
    auto hashedData = hasher.hash(data);
    std::cout << "Data hashed successfully using algorithm: " << static_cast<int>(hasher.getAlgorithm()) << std::endl;
}

int main(int argc, char *argv[])
{
    MyData data("Hello, World!");
    MyHasher<MyData> hasher;

    processHash(data, hasher); // Используем функцию с хешером и данными

    // MyData1 data1("Hello, World!");
    // MyHasher<MyData1> hasher1; // the required expression ‘t.size()’ is invalid

    // MyData data1("Hello, World!");
    // MyHasher1<MyData> hasher1;

    // processHash(data, hasher1); // the required expression ‘h.getAlgorithm()’ is invalid

    //    fu();
    // proccess p{};
    // if (p.arg.parse(argc, argv) != 0)
    // {
    //     return EXIT_FAILURE;
    // }
    // p.hash_function = std::shared_ptr<hash::HashImpl>(std::move(hash::HashImpl::Create(p.arg.hashes)));
    // p.arg.directory.emplace_back("/home/sasha/Documents/1");
    // p.arg.directory.emplace_back("/home/sasha/Documents/1");
    // auto dir_except = std::make_shared<DirectoryException>();
    // std::ranges::for_each(p.arg.directory_exception,
    //                       [&dir_except](const std::string &v) noexcept { dir_except->insert(v); });
    // dir_except->rehash(dir_except->size());
    // std::ranges::for_each(p.arg.directory,
    //                       [&dir_except, &p](const std::string &v) noexcept
    //                       {
    //                           // TODO: thread - may be
    //                           try
    //                           {
    //                               p.file_finder(v, dir_except);
    //                           }
    //                           catch (const std::filesystem::filesystem_error &ex)
    //                           {
    //                               std::cout << "what():  " << ex.what() << '\n';
    //                           }
    //                           catch (...)
    //                           {
    //                               std::cout << "error: \n";
    //                           };
    //                       });
    return EXIT_SUCCESS;
}
