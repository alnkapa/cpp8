#include <boost/multi_index_container.hpp>
#include <filesystem>
#include <memory>
#include <unordered_set>

#include "argc/argc.h"
#include "global/hash.h"
#include "hashing/hash.h"

using namespace boost::multi_index;
using DirectoryException = std::unordered_set<std::string>;
struct proccess {
    std::unique_ptr<::hash::Hash> hash_function;
    arg::Argc arg;
    std::size_t get_block_number(const std::uintmax_t& file_size, const std::size_t& block_size) noexcept {
        auto block_number = file_size / block_size;
        if (file_size % block_size != 0) {
            block_number++;
        }
        return block_number;
    }
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
                std::cout << " filename :" << it->path().filename() << "\n";
                // 0) если индекс пустой, записать файл в индекс и пропустить
                //
                // 1) найти файл в индексе если нет, да файл уже проиндексирован пропустить
                // 2) читаем по блочно файл
                //  1) делаем блок
                //  2) получаем все блоки из индекса с таким номером и таким значение хеша
                //  3) если не одного не найден, сохранить файл в индекс и пропустить
                // 3) если все блоки совпали, то записываем имя этого и совпавшего файла
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
    p.hash_function = hash::HashImpl::Create(p.arg.hashes);
    
    p.arg.directory.emplace_back("CMakeFiles");
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
