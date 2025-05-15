#ifndef BROADCAST_UTILITIES_HASH_HPP
#define BROADCAST_UTILITIES_HASH_HPP

#include <cstdint>
#include <utility>
#include <cstddef>

namespace broadcast {

/**
 * @brief Provides a high-quality and fast custom hash function using splitmix64.
 */
struct customHash {
    size_t operator()(uint64_t x) const;
    size_t operator()(const std::pair<int, int>& p) const;

private:
    static uint64_t splitmix64(uint64_t x);
};

} // namespace broadcast

#endif // BROADCAST_UTILITIES_HASH_HPP
