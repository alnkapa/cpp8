#include "argc.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

std::istream& operator>>(std::istream& in, Argc::Hashes& in) {
    // TODO: insert return statement here
    return in;
}

int Argc::parse(int argc, char* argv[]) {
    namespace po = boost::program_options;
    // clang-format off
    // Определяем параметры командной строки
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    // ("directory,d", po::value<std::vector<std::string>>(&directory)->required(),"input directory")
    // ("exception,e", po::value<std::vector<std::string>>(&directory),"exception directory")
    ("level,l", po::value<int>(&deep_level)->default_value(0),"deep level")
    ("size,s", po::value<std::size_t>(&file_size)->default_value(1),"min file size in byte")
    ("wildcards,w", po::value<std::string>(&wildcards),"file name wildcards")
    ("bsize,S", po::value<std::size_t>(&block_size)->default_value(1), "block size")
    ("hashes,H", po::value<Argc::Hashes>(&hashes)->default_value(Argc::Hashes::crc32), "hash algorithm");
    ;
    // clang-format on

    // Обрабатываем аргументы командной строки
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::required_option& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return {};
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return {};
    }
    // Выводим значения аргументов
    // std::copy(directory.begin(), directory.end(), std::ostream_iterator<int>(std::cout, " "));
    // std::cout << std::endl;
    // std::copy(directory_exception.begin(), directory_exception.end(), std::ostream_iterator<int>(std::cout, " "));
    // std::cout << std::endl;
    std::cout << "level:" << deep_level << std::endl;
    std::cout << "size:" << file_size << std::endl;
    std::cout << "block:" << block_size << std::endl;
    std::cout << static_cast<int>(hashes) << std::endl;
    return {};
};
