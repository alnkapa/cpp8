#include <filesystem>
#include <memory>
#include <unordered_set>

#include "argc/argc.h"
#include "global/hash.h"
#include "hashing/hash.h"

using DirectoryException = std::unordered_set<std::string>;
struct proccess {    
    arg::Argc arg;
    // просмотр файловой системы
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
                // auto block_number = get_block_number(it->file_size(), arg.block_size);
                // std::cout << "block_number:" << block_number << " file_size:" << it->file_size() << "\n";
                // std::cout << "read siZe: ";
                //  for (auto i = 0; i < block_number; ++i) {
                //      try {
                //          get_block_hash(it->path(), it->file_size(), i, arg.block_size);
                //      } catch (const std::exception& ex) {
                //          std::cout << "what():  " << ex.what() << '\n';
                //      }
                //  };
                std::cout << '\n';
                // std::terminate();
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
