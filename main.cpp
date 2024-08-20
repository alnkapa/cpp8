#include <filesystem>
#include "argc/argc.h"
#include "global/hash.h"
#include "hashing/hash.h"
#include "file_finder.h"

int main(int argc, char *argv[])
{
    proccess<hash::HashTypeImpl> p;
    if (p.arg.parse(argc, argv) != 0)
    {
        return EXIT_FAILURE;
    }

    // DEBUG
    p.arg.directory.emplace_back("/home/sasha/Documents/1");
    p.arg.directory.emplace_back("/home/sasha/Documents/1");
    // DEBUG

    p.h_fu = hash::CreateHasher<hash::HashTypeImpl>(p.arg.hashes);

    auto dir_except = std::make_shared<DirectoryException>();
    std::ranges::for_each(p.arg.directory_exception,
                          [&dir_except](const std::string &v) noexcept { dir_except->insert(v); });
    dir_except->rehash(dir_except->size());

    std::ranges::for_each(p.arg.directory,
                          [&dir_except, &p](const std::string &v) noexcept
                          {
                              // TODO: thread - may be
                              try
                              {
                                  p.file_finder(v, dir_except);
                              }
                              catch (const std::filesystem::filesystem_error &ex)
                              {
                                  std::cout << "what():  " << ex.what() << '\n';
                              }
                              catch (...)
                              {
                                  std::cout << "error: \n";
                              };
                          });
    return EXIT_SUCCESS;
}
