#ifndef GLOBAL_H
#define GLOBAL_H
#include <string>

namespace hash {
    // Перечисление для алгоритмов хеширования
    enum class Algorithm { SHA256, MD5, SHA1, CRC32 };

    // тип данных для хранения принимаемого и передаваемого значения
    using HashType = std::string;

}  // namespace hash

#endif  // GLOBAL_H