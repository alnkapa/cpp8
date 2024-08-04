#include <algorithm>
#include <boost/multi_index_container.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>

#include "argc/argc.h"
using namespace boost::multi_index;

std::size_t get_block_number(std::uintmax_t file_size, std::size_t block_size) {
    auto block_number = file_size / block_size;
    if (file_size % block_size != 0) {
        block_number++;
    }
    return block_number;
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
            my_file.close();
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
                std::fstream my_file(it->path());
                if (!my_file) {
                    std::cout << "can`t open file:" << it->path();
                }
                std::uint8_t buf[arg.block_size]{};
                std::cout << "read siZe: ";
                for (auto i = 0; i < block_number; ++i) {
                    try {
                        my_file.seekg(i * arg.block_size);
                        std::cout << my_file.tellg() << " " << i * arg.block_size << " : ";
                        my_file.read(reinterpret_cast<char*>(&buf), arg.block_size);
                        std::cout << my_file.gcount() << " ";
                    } catch (const std::exception& ex) {
                        break;
                        std::cout << "what():  " << ex.what() << '\n';                    
                    }
                    // Do hash
                }
                std::cout << '\n';
                my_file.close();
                //std::terminate();
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
