#ifndef HASH_H
#define HASH_H
#include <memory>
#include <openssl/evp.h>
#include <boost/crc.hpp>
#include "../global/hash.h"
#include "hash_type.h"

namespace hash
{
template <typename HashTypeImpl> class HasherBase
{
  public:
    virtual ~HasherBase() {};
    virtual HashTypeImpl hash(const HashTypeImpl &) const = 0;
    virtual Algorithm getAlgorithm() const noexcept = 0;
};

template <typename HashTypeImpl, Algorithm Alg> class HashImpl : public HasherBase<HashTypeImpl>
{
  private:
    // хеш-функция
    const EVP_MD *m_hash;

  public:
    HashImpl()
    {
        if constexpr (Alg == Algorithm::SHA256)
        {
            m_hash = EVP_sha256();
        }
        else if constexpr (Alg == Algorithm::MD5)
        {
            m_hash = EVP_md5();
        }
        else if constexpr (Alg == Algorithm::SHA1)
        {
            m_hash = EVP_sha1();
        }
        else
        {
            // https://stackoverflow.com/a/64354296
            []<bool flag = false>() { static_assert(flag, "Bad Algorithm"); }();
            // static_assert(Alg == Algorithm::SHA256 || Alg == Algorithm::MD5 || Alg == Algorithm::SHA1, "Bad
            // Algorithm");
        }
    };
    inline Algorithm getAlgorithm() const noexcept
    {
        return Alg;
    };
    HashTypeImpl hash(const HashTypeImpl &in) const
    {
        // хеш-значения
        value_type value[EVP_MAX_MD_SIZE];
        // хеш-длина
        size_type len{0};
        // Освобождаем контекст по выходу
        struct EVP_MD_CTX_Deleter
        {
            void operator()(EVP_MD_CTX *context) const
            {
                EVP_MD_CTX_free(context);
            }
        };
        // контекст для хеширования
        std::unique_ptr<EVP_MD_CTX, EVP_MD_CTX_Deleter> context(EVP_MD_CTX_new());
        if (!context)
        {
            throw std::runtime_error("Failed to create EVP_MD_CTX");
        }

        // инициализация
        if (EVP_DigestInit_ex(context.get(), m_hash, NULL) != 1)
        {
            throw std::runtime_error("Failed to initialize digest");
        }

        // хеширование
        if (EVP_DigestUpdate(context.get(), in.data(), in.size()) != 1)
        {
            throw std::runtime_error("Failed to update digest");
        }

        // результат
        if (EVP_DigestFinal_ex(
                context.get(), reinterpret_cast<unsigned char *>(&value), reinterpret_cast<unsigned int *>(&len)) != 1)
        {
            throw std::runtime_error("Failed to finalize digest");
        }
        return {value, len};
    };
};
template <> class HashImpl<HashTypeImpl, Algorithm::CRC32> : public HasherBase<HashTypeImpl>
{
  public:
    inline Algorithm getAlgorithm() const noexcept
    {
        return Algorithm::CRC32;
    };
    HashTypeImpl hash(const HashTypeImpl &in) const
    {
        boost::crc_32_type crc;
        crc.process_bytes(in.data(), in.size());
        const size_t size = sizeof(value_type);
        union
        {
            unsigned int rez;
            value_type buf[size];
        } val;
        val.rez = crc.checksum();
        return {val.buf, size};
    };
};

static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::SHA256>>,
              "Hasher does not satisfy Hasher concept");
static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::MD5>>, "Hasher does not satisfy Hasher concept");
static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::SHA1>>, "Hasher does not satisfy Hasher concept");
static_assert(Hasher<HashTypeImpl, HashImpl<HashTypeImpl, Algorithm::CRC32>>, "Hasher does not satisfy Hasher concept");

// runtime only
template <typename HashTypeImpl> std::unique_ptr<HasherBase<HashTypeImpl>> CreateHasher(Algorithm al)
{
    switch (al)
    {
    case Algorithm::SHA256:
        return std::make_unique<HashImpl<HashTypeImpl, Algorithm::SHA256>>();
    case Algorithm::MD5:
        return std::make_unique<HashImpl<HashTypeImpl, Algorithm::MD5>>();
    case Algorithm::SHA1:
        return std::make_unique<HashImpl<HashTypeImpl, Algorithm::SHA1>>();
    case Algorithm::CRC32:
    default:
        return std::make_unique<HashImpl<HashTypeImpl, Algorithm::CRC32>>();
    }
};

} // namespace hash

#endif // HASH_H

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

// // Класс, представляющий данные
// class MyData
// {
//   public:
//     MyData(const std::string &data) : data_(data)
//     {
//     }

//     MyData(const unsigned char *, std::size_t)
//     {
//     }

//     std::size_t size() const
//     {
//         return data_.size();
//     }

//     const unsigned char *data() const
//     {
//         return reinterpret_cast<const unsigned char *>(data_.data());
//     }

//   private:
//     std::string data_;
// };

// // Класс, представляющий данные
// class MyData1
// {
//   public:
//     MyData1(const std::string &data) : data_(data)
//     {
//     }

//     // std::size_t size() const
//     // {
//     //     return data_.size();
//     // }

//     const unsigned char *data() const
//     {
//         return reinterpret_cast<const unsigned char *>(data_.data());
//     }

//   private:
//     std::string data_;
// };

// template <typename T>
//     requires hash::HashType<T>
// class MyHasher
// {
//   public:
//     T hash(const T &data)
//     {
//         data.size();
//         data.data();
//         size_t block_size{10};
//         unsigned char buf[block_size]{0};
//         return {reinterpret_cast<const unsigned char *>(&buf), block_size};
//     }

//     hash::Algorithm getAlgorithm() const
//     {
//         return hash::Algorithm::SHA256;
//     }
// };

// template <typename T>
//     requires hash::HashType<T>
// class MyHasher1
// {
//   public:
//     T hash(const T &data)
//     {
//         data.size();
//         data.data();
//         return data;
//     }

//     // hash::Algorithm getAlgorithm() const
//     // {
//     //     return hash::Algorithm::SHA256;
//     // }
// };

// // Шаблонная функция, использующая концепты
// template <typename T, typename H>
//     requires hash::HashType<T> && hash::Hasher<T, H>
// void processHash(T data, H hasher)
// {
//     auto hashedData = hasher.hash(data);
//     std::cout << "Data hashed successfully using algorithm: " << static_cast<int>(hasher.getAlgorithm()) <<
//     std::endl;
// }

// MyData data("Hello, World!");
// MyHasher<MyData> hasher;

// processHash(data, hasher); // Используем функцию с хешером и данными

// MyData1 data1("Hello, World!");
// MyHasher<MyData1> hasher1; // the required expression ‘t.size()’ is invalid

// MyData data1("Hello, World!");
// MyHasher1<MyData> hasher1;

// processHash(data, hasher1); // the required expression ‘h.getAlgorithm()’ is invalid

//    fu();
