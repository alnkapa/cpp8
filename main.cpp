#include <filesystem>
#include <memory>
#include <unordered_set>

#include "argc/argc.h"

using DirectoryException = std::unordered_set<std::string>;

// file_finder
// deep_level - глубина рекурсии
// directory_exception - директории для исключения из сканирования
int file_finder(std::shared_ptr<DirectoryException> directory_exception, int deep_level) {
    namespace fs = std::filesystem;
    fs::path dir("CMakeFiles");
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        return -1;
    }
    for (auto it = fs::recursive_directory_iterator(dir); it != fs::recursive_directory_iterator(); ++it) {
        if (it.depth() + 1 > deep_level) {
            it.disable_recursion_pending();
        }        
        std::cout << it.depth() << *it << "\n";
        // if (fs::is_regular_file(entry)) {
        //     std::cout << "Найден файл: " << entry.path() << std::endl;
        // }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    arg::Argc arg{};
    if (arg.parse(argc, argv) != 0) {
        return EXIT_FAILURE;
    }
    auto dir_except = std::make_shared<DirectoryException>();
    for (const auto &v: arg.directory_exception) {        
    };
    file_finder(dir_except, arg.deep_level);

    return EXIT_SUCCESS;
}
