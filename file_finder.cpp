#include "file_finder.h"
#include <iostream>
#include <fstream>
#include <memory>

inline std::size_t get_block_number(std::uintmax_t file_size, std::size_t block_size) noexcept
{
    auto block_number = file_size / block_size;
    if (file_size % block_size != 0)
    {
        block_number++;
    }
    return block_number;
}

struct BlockSave
{
    BlockSave(const size_t index, const hash::HashTypeImpl &value) : m_index(index), m_value(value)
    {
    }

    void operator()(files::File &e)
    {
        e.assign(m_index, m_value);
    }

  private:
    size_t m_index;
    hash::HashTypeImpl m_value;
};

hash::HashTypeImpl proccess::block_hash(const std::string &path, const size_t b_num)
{
    auto file = std::fstream(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path);
    }
    char buf[m_block_size];
    file.seekg(b_num * m_block_size);
    auto size = file.read(reinterpret_cast<char *>(&buf), m_block_size).gcount();
    if (size == 0 || size > m_block_size)
    {
        throw std::length_error("block size mismatch: " + std::to_string(size));
    }
    return h_fu->hash({reinterpret_cast<hash::value_type *>(&buf), static_cast<hash::size_type>(size)});
}

bool proccess::block_cmp(const std::string &path1, const std::string &path2, std::uintmax_t file_size)
{
    // по блочное сравнение
    auto block_number = get_block_number(file_size, m_block_size);
    auto &file_index = container.get<0>();
    auto file_name_it = file_index.find(path2);
    if (file_name_it == file_index.end())
    {
        throw std::invalid_argument("file not fount: " + path2);
    }
    for (std::size_t b_num = 0; b_num < block_number; ++b_num)
    {
        std::cout << "BLOCK: " << b_num << "\n";
        if (!file_name_it->is_block(b_num))
        {
            file_index.modify(file_name_it, BlockSave(b_num, block_hash(path1, b_num)));
        }
        try
        {
            m_blocks_path1.at(b_num);
        }
        catch (const std::out_of_range &ex)
        {
            m_blocks_path1.assign(b_num, block_hash(path1, b_num));
        }
        if (m_blocks_path1[b_num] != (*file_name_it)[b_num])
        {
            // блоки не совпали
            return false;
        }
    }
    return true;
}

void proccess::file_cmp(const std::string &path, std::uintmax_t file_size)
{
    // найти похожие файлы по размеру
    auto &file_size_index = container.get<1>();
    auto file_size_range_it = file_size_index.equal_range(file_size);
    if (file_size_range_it.first == file_size_range_it.second)
    {
        // TODO: добавить в индекс сравниваемый файл
        // совпадений по размеру не найдено
        return;
    }
    std::cout << "COMP1: File: " << path << ", Size: " << file_size << "\n";
    m_blocks_path1.clear();
    for (auto &file_size_it = file_size_range_it.first; file_size_it != file_size_range_it.second; ++file_size_it)
    {
        std::cout << "COMP2: File: " << file_size_it->path << ", Size: " << file_size_it->size << "\n";
        // TODO: parallel ?
        block_cmp(path, file_size_it->path, file_size);
    }
}
