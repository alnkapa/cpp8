#include "argc.h"

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace arg {
    int Argc::parse(int argc, char* argv[]) {
        namespace po = boost::program_options;
        // clang-format off
        // Определяем параметры командной строки
        po::options_description desc("Allowed options");
        desc.add_options()
        (
            "help,h", 
            "produce help message"
        )
        (
            "directory,d", 
            po::value<std::vector<std::string>>(&directory),
            "input directory (for multiples input use -d dir1 -d dir2 -d dir3 ...)"
        )
        (
            "exception,e", 
            po::value<std::vector<std::string>>(&directory_exception),
            "exception directory (for multiples input use -e dir1 -e dir2 -e dir3 ...)"
        )
        (
            "level,l", 
            po::value<int>(&deep_level)->default_value(0),
            "deep level"
        )
        (
            "size,s", 
            po::value<std::size_t>(&file_size)->default_value(1),
            "min file size in byte"
        )
        (
            "wildcards,w", 
            po::value<std::string>(&wildcards),
            "file name wildcards variants *.cpp, file?.txt, file[1-3].txt"
        )
        (
            "bsize,S", 
            po::value<std::size_t>(&block_size)->default_value(1), 
            "block size"
        )
        (
            "hashes,H", 
            po::value<Argc::Hashes>(&hashes)->default_value(Argc::Hashes::crc32), 
            "hash algorithm (crc32 or md5)"
        );
        // clang-format on
        po::variables_map vm;
        try {
            po::store(po::parse_command_line(argc, argv, desc), vm);
            po::notify(vm);
            if (vm.count("help")) {
                std::cout << desc << "\n";
                return -2;
            }
        } catch (const po::required_option& e) {
            std::cerr << "Error: " << e.what() << "\n";
            std::cerr << desc << "\n";
            return -1;
        } catch (const po::error& e) {
            std::cerr << "Error: " << e.what() << "\n";
            std::cerr << desc << "\n";
            return -1;
        }
        // Выводим значения аргументов
        std::cout << "Input directories: ";
        std::copy(directory.begin(), directory.end(), std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << "\n";
        if (!directory_exception.empty()) {
            std::cout << "Exception directories: ";
            std::copy(directory_exception.begin(), directory_exception.end(),
                      std::ostream_iterator<std::string>(std::cout, " "));
        }
        std::cout << "\n";
        std::cout << "Level: " << deep_level << "\n";
        std::cout << "Size: " << file_size << "\n";
        std::cout << "Block size: " << block_size << "\n";
        std::cout << "Hash algorithm: " << boost::lexical_cast<std::string>(hashes) << "\n";
        return 0;
    };
};  // namespace arg
