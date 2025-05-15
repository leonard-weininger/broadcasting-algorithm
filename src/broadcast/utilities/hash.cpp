#include <broadcast/utilities/hash.hpp>

#include <chrono>

namespace broadcast {

uint64_t customHash::splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

size_t customHash::operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM =
        std::chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
}

size_t customHash::operator()(const std::pair<int, int>& p) const {
    uint64_t key = (uint64_t(uint32_t(p.first)) << 32) | uint32_t(p.second);
    return operator()(key);
}

} // namespace broadcast
