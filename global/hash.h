#ifndef GLOBAL_H
#define GLOBAL_H
#include <string>

namespace hash {
    // Перечисление для алгоритмов хеширования
    enum class Algorithm { SHA256, MD5, SHA1, CRC32 };

    // тип данных для хранения принимаемого и передаваемого значения
    using HashType = std::string;

    class Hash {
       public:
        virtual HashType hash(const HashType&) = 0;
        virtual hash::Algorithm getAlgorithm() const = 0;
        virtual ~Hash() {};
    };
}  // namespace hash

#endif  // GLOBAL_H