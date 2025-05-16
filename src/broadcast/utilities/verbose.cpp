#include <broadcast/utilities/verbose.hpp>

#include <sstream>
#include <string>

namespace broadcast {

const int LABEL_WIDTH = 37;
const int VALUE_WIDTH = 20;

void printInfo(const std::string& label, const std::string& value, const std::string& unit) {
    std::ostringstream oss;
    oss << std::left << std::setw(LABEL_WIDTH) << std::setfill('.') << label;

    std::string formattedLabel = oss.str();
    std::cerr << "[INFO] " << formattedLabel;
    std::cerr << std::right << std::setw(VALUE_WIDTH) << std::setfill('.');
    std::cerr << value;
    if (!unit.empty()) std::cerr << ' ' << unit;
    std::cerr << '\n';
}

void printInfo(const std::string& label, double seconds, int precision) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << seconds;
    printInfo(label, out.str(), "seconds");
}

void printInfo(const std::string& label, int value) {
    printInfo(label, std::to_string(value));
}

void printInfo(const std::string& label, long long value) {
    printInfo(label, std::to_string(value));
}

void printInfo(const std::string& label, unsigned long long value) {
    printInfo(label, std::to_string(value));
}

void printEmptyLine() {
    std::cerr << "[INFO]\n";
}

void printHeader(const std::string& message) {
    std::ostringstream oss;
    oss << std::left << std::setw(LABEL_WIDTH + VALUE_WIDTH) << std::setfill('.') << message;
    std::string formattedLabel = oss.str();
    std::cerr << "[INFO] " << formattedLabel << '\n';
}

} // namespace broadcast

