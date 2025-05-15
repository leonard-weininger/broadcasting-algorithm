#include <broadcast/utilities/timer.hpp>

namespace broadcast {

Timer::Timer() : start_time(std::chrono::steady_clock::now()) {}

void Timer::reset() {
    start_time = std::chrono::steady_clock::now();
}

double Timer::elapsed() const {
    using namespace std::chrono;
    return duration_cast<duration<double>>(steady_clock::now() - start_time).count();
}

} // namespace broadcast

