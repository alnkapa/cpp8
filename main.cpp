#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/key.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <tuple>
#include <unordered_set>

#include "argc/argc.h"
#include "files/files.h"
#include "global/hash.h"
#include "hashing/hash.h"

using namespace boost::multi_index;
using DirectoryException = std::unordered_set<std::string>;

struct file_extractor {
    typedef std::string result_type;
    const result_type& operator()(const files::File& e) const { return e.get_path(); }
    result_type& operator()(files::File* e) const { return e->path(); }
};

struct file_size_extractor {
    typedef std::uintmax_t result_type;
    const result_type& operator()(const files::File& e) const { return e.get_file_size(); }
    result_type& operator()(files::File* e) const { return e->file_size(); }
};
// clang-format off
typedef multi_index_container<
  files::File,
  indexed_by<
    hashed_unique<file_extractor>, // поиск файла 
    ordered_non_unique<file_size_extractor> // поиск размера     
  >
> container;

// clang-format on
struct proccess {
    std::shared_ptr<::hash::Hash> hash_function;
    arg::Argc arg;
    container index;

    std::size_t get_block_number(std::uintmax_t file_size, std::size_t block_size) noexcept {
        auto block_number = file_size / block_size;
        if (file_size % block_size != 0) {
            block_number++;
        }
        return block_number;
    }
    ::hash::HashType get_block_hash(const std::string& path, const size_t block_index, const size_t block_size) {
        auto file = std::fstream(path, std::ios::in | std::ios::binary);
        char buf[block_size]{};
        file.seekg(block_index * block_size);
        std::cout << file.tellg() << " " << block_index * block_size << " : ";
        auto size = file.read(reinterpret_cast<char*>(&buf), block_size).gcount();
        if (size == 0 || size > block_size) {
            throw(std::length_error("block size mismatch: " + size));
        }
        std::cout << size << " ";
        return hash_function->hash({reinterpret_cast<const char*>(&buf), std::size_t(size)});
    }

    void add_to_index(const std::string& file_name, std::uintmax_t file_size, std::size_t block_numbers) {
        std::cout << "IN: File: " << file_name << ", Size: " << file_size << "\n";
        index.emplace(file_name, file_size, arg.block_size, block_numbers);
    }

    // просмотр файловой системы
    void file_finder(const std::string& path, std::shared_ptr<DirectoryException> directory_exception) {
        namespace fs = std::filesystem;
        fs::path dir(path);
        auto& file_index = index.get<0>();
        auto& file_size_index = index.get<1>();
        container::iterator p;
        for (auto dir_it = fs::recursive_directory_iterator(dir); dir_it != fs::recursive_directory_iterator();
             ++dir_it) {
            if (dir_it->is_directory()) {
                if (dir_it.depth() + 1 > arg.deep_level) {
                    dir_it.disable_recursion_pending();
                } else if (directory_exception->contains(dir_it->path().filename())) {
                    dir_it.disable_recursion_pending();
                }
            } else if (dir_it->is_regular_file() && dir_it->file_size() >= arg.file_size) {
                auto block_number = get_block_number(dir_it->file_size(), arg.block_size);
                if (index.empty()) {
                    // индекс пустой
                    add_to_index(dir_it->path(), dir_it->file_size(), block_number);
                    continue;
                }
                if (auto file_name_it = file_index.find(dir_it->path()); file_name_it != file_index.end()) {
                    // файл уже проиндексирован
                    continue;
                }
                auto file_size_range_it = file_size_index.equal_range(dir_it->file_size());
                if (file_size_range_it.first == file_size_range_it.second) {
                    // совпадений по размеру не найдено
                    add_to_index(dir_it->path(), dir_it->file_size(), block_number);
                    continue;
                }
                std::cout << "COMP: File: " << dir_it->path() << ", Size: " << dir_it->file_size() << "\n";
                files::BlockVector blocks{block_number};
                // по блочное сравнение
                for (std::size_t b_num = 0; b_num < block_number; ++b_num) {
                    std::cout << "BLOCK: " << b_num << "\n";
                    // читаем очередной блок
                    auto c_bb = blocks[b_num] = get_block_hash(dir_it->path(), b_num, arg.block_size);
                    // по каждому файлу, который совпадает по размеру
                    for (auto file_size_it = file_size_range_it.first; file_size_it != file_size_range_it.second;
                         ++file_size_it) {
                        std::cout << "FIND: File: " << file_size_it->get_path() << "\n";
                        auto file_blocks = file_size_it->blocks();
                        ::hash::HashType f_bb;
                        try {
                            f_bb = file_blocks.at(b_num);
                        } catch (const std::out_of_range& ex) {
                            f_bb = file_blocks[b_num] = get_block_hash(file_size_it->get_path(), b_num, arg.block_size);
                        }
                        if (f_bb != c_bb) {  // сравнение блоков
                            // сохранить все ранее прочитанные блоки для этого файла
                            // исключить его их дальнейшего сравнения
                        }
                    }
                    if (b_num == 2) {
                        std::terminate();
                    }
                }                
            }
        };
    }
};

int main(int argc, char* argv[]) {
    proccess p{};
    if (p.arg.parse(argc, argv) != 0) {
        return EXIT_FAILURE;
    }
    p.hash_function = std::shared_ptr<hash::HashImpl>(std::move(hash::HashImpl::Create(p.arg.hashes)));
    p.arg.directory.emplace_back("/home/sasha/Music");
    p.arg.directory.emplace_back("/home/sasha/Music");
    auto dir_except = std::make_shared<DirectoryException>();
    std::ranges::for_each(p.arg.directory_exception,
                          [&dir_except](const std::string& v) noexcept { dir_except->insert(v); });
    dir_except->rehash(dir_except->size());
    std::ranges::for_each(p.arg.directory, [&dir_except, &p](const std::string& v) noexcept {
        // TODO: thread - may be
        try {
            p.file_finder(v, dir_except);
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cout << "what():  " << ex.what() << '\n';
        } catch (...) {
            std::cout << "error: \n";
        };
    });
    return EXIT_SUCCESS;
}
