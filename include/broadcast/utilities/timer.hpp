#ifndef BROADCAST_UTILITIES_TIMER_HPP
#define BROADCAST_UTILITIES_TIMER_HPP

#include <chrono>

namespace broadcast {

/**
 * @brief Timer class for measuring elapsed time in seconds.
 * 
 * Use `reset()` to restart the timer and `elapsed()` to get the
 * time since the last reset or construction.
 */
class Timer {
public:
    Timer();

    /// Resets the timer to the current time.
    void reset();

    /// Returns the elapsed time in seconds since construction or last reset.
    double elapsed() const;

private:
    std::chrono::steady_clock::time_point start_time;
};

} // namespace broadcast

#endif // BROADCAST_UTILITIES_TIMER_HPP

