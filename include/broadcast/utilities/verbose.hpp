#ifndef BROADCAST_UTILITIES_VERBOSE_HPP
#define BROADCAST_UTILITIES_VERBOSE_HPP

#include <iostream>
#include <iomanip>
#include <string>

namespace broadcast {

/**
 * @brief Provides functions for printing formatted diagnostic information.
 *
 * These functions are used to output information about the algorithm's progress,
 * including runtime, number of spanning trees, and other key metrics. They are
 * only triggered when the verbose mode is enabled.
 */
void printInfo(const std::string& label, const std::string& value, const std::string& unit = "");
void printInfo(const std::string& label, double seconds, int precision = 4);
void printInfo(const std::string& label, int value);
void printInfo(const std::string& label, long long value);
void printInfo(const std::string& label, unsigned long long value);
void printEmptyLine();
void printHeader(const std::string& message);

} // namespace broadcast

#endif // BROADCAST_UTILITIES_VERBOSE_HPP

