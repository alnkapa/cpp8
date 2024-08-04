#include <algorithm>
#include <filesystem>
#include <memory>
#include <unordered_set>

#include "argc/argc.h"

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
            } else if (it->is_regular_file()) {
                std::cout << it.depth() << " : " << *it << "\n";
                std::cout << " Directory : " << it->path().root_directory() << "\n"
                          << "root_name :" << it->path().root_name() << "\n"
                          << "root_path :" << it->path().root_path() << "\n"
                          << "relative_path :" << it->path().relative_path() << "\n"
                          << "parent_path :" << it->path().parent_path() << "\n"
                          << "filename :" << it->path().filename() << "\n"
                          << "stem :" << it->path().stem() << "\n"
                          << "extension :" << it->path().extension() << "\n"
                          << "\n";
            }
        };
    }
};
int main(int argc, char* argv[]) {
    proccess p{};
    if (p.arg.parse(argc, argv) != 0) {
        return EXIT_FAILURE;
    }
    auto dir_except = std::make_shared<DirectoryException>();
    std::ranges::for_each(p.arg.directory_exception, [&dir_except](const std::string& v) { dir_except->insert(v); });
    std::ranges::for_each(p.arg.directory, [&dir_except, &p](const std::string& v) {
        try {
            p.file_finder(v, dir_except);
        } catch (std::filesystem::filesystem_error const& ex) {
            std::cout << "what():  " << ex.what() << '\n';
        };
    });
    return EXIT_SUCCESS;
}
