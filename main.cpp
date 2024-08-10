#include <algorithm>
#include <boost/multi_index_container.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>

#include "argc/argc.h"
#include "hashing/hash.h"
using namespace boost::multi_index;

std::size_t get_block_number(std::uintmax_t file_size, std::size_t block_size) {
    auto block_number = file_size / block_size;
    if (file_size % block_size != 0) {
        block_number++;
    }
    return block_number;
}
// TODO:  привязать к std::unique_ptr
void close_file(std::fstream* ptr) {
    if (ptr->is_open()) ptr->close();
};

std::size_t get_block_hash(const std::string& path, std::uintmax_t file_size, std::size_t block_index,
                           std::size_t block_size) {
    std::unique_ptr<std::fstream, decltype(&close_file)> file_ptr(
        new std::fstream(path, std::ios::in | std::ios::binary), &close_file);
    if (!file_ptr->is_open()) {
        throw(std::ios_base::failure("can`t open file: " + path));
    }
    auto h = Hash::createHash(HashAlgorithm::MD5);
    char buf[block_size]{};
    file_ptr->seekg(block_index * block_size);
    std::cout << file_ptr->tellg() << " " << block_index * block_size << " : ";
    auto size = file_ptr->read(reinterpret_cast<char*>(&buf), block_size).gcount();
    std::cout << size << " ";
    h->hash(buf);

    // Do hash
    return {0};
}

template <typename T>
class Block {
   private:
    std::string m_path;          // путь до файла
    std::size_t m_index;         // номер блока в файле
    std::uintmax_t m_file_size;  // размер файла
    T m_hash;                    // хеш
   public:
    Block(const std::string& path, const std::size_t& index, std::uintmax_t file_size, const T& hash)
        : m_path(path), m_index(index), m_file_size(file_size), m_hash(hash){};
    std::size_t index() { return m_index; };
    T hash() {
        if (m_hash.size() == 0) {
            std::fstream my_file(m_path);
            if (!my_file) {
                throw(std::filesystem::filesystem_error("can`t open file", std::filesystem::path(m_path)));
            }
            return m_hash;
        } else {
            return m_hash;
        }
    };
};

using DirectoryException = std::unordered_set<std::string>;
struct proccess {
    arg::Argc arg;
    void file_finder(const std::string& path, std::shared_ptr<DirectoryException> directory_exception) {
        namespace fs = std::filesystem;
        fs::path dir(path);
        for (auto it = fs::recursive_directory_iterator(dir); it != fs::recursive_directory_iterator(); ++it) {
            if (it->is_directory()) {
                if (it.depth() + 1 > arg.deep_level) {
                    it.disable_recursion_pending();
                } else if (directory_exception->contains(it->path().filename())) {
                    it.disable_recursion_pending();
                }
            } else if (it->is_regular_file() && it->file_size() >= arg.file_size) {
                std::cout << "depth:" << it.depth() << " filename :" << it->path().filename() << "\n";
                auto block_number = get_block_number(it->file_size(), arg.block_size);
                std::cout << "block_number:" << block_number << " file_size:" << it->file_size() << "\n";
                std::cout << "read siZe: ";
                for (auto i = 0; i < block_number; ++i) {
                    try {
                        get_block_hash(it->path(), it->file_size(), i, arg.block_size);
                    } catch (const std::exception& ex) {
                        std::cout << "what():  " << ex.what() << '\n';
                    }
                };
                std::cout << '\n';
                std::terminate();
            }
        };
    }
};

int main(int argc, char* argv[]) {
    proccess p{};
    if (p.arg.parse(argc, argv) != 0) {
        return EXIT_FAILURE;
    }
    p.arg.directory.emplace_back("CMakeFiles");
    auto dir_except = std::make_shared<DirectoryException>();
    std::ranges::for_each(p.arg.directory_exception,
                          [&dir_except](const std::string& v) noexcept { dir_except->insert(v); });
    dir_except->rehash(dir_except->size());
    std::ranges::for_each(p.arg.directory, [&dir_except, &p](const std::string& v) noexcept {
        // TODO: thread
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
