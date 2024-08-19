#ifndef GLOBAL_H
#define GLOBAL_H
#include <concepts>

namespace hash
{
// Перечисление для алгоритмов хеширования
enum class Algorithm
{
    SHA256,
    MD5,
    SHA1,
    CRC32
};

template <typename T>
concept HashType = requires(T a) {
    { a.size() } -> std::convertible_to<std::size_t>;
    { a.data() } -> std::convertible_to<unsigned char *>;
};

template <typename T, typename U>
concept Hash = HashType<U> && requires(T a, U b) {
    { a.hash() } -> std::convertible_to<U>;
    { a.getAlgorithm() } -> std::convertible_to<Algorithm>;
};

} // namespace hash

#endif // GLOBAL_H