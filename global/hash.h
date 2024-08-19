#ifndef GLOBAL_H
#define GLOBAL_H
#include <concepts>
#include <cstddef>

namespace hash
{
enum class Algorithm
{
    SHA256,
    MD5,
    SHA1,
    CRC32
};

template <typename T>
concept HashType = requires(T t) {
    {
        t.size()
    } -> std::convertible_to<std::size_t>;
    {
        t.data()
    } -> std::convertible_to<const unsigned char *>;
};

template <typename T, typename H>
concept Hasher = HashType<T> && requires(const T &t, H h) {
    {
        h.hash(t)
    } -> std::convertible_to<T>;
    {
        h.getAlgorithm()
    } -> std::convertible_to<Algorithm>;
};

} // namespace hash

#endif // GLOBAL_H