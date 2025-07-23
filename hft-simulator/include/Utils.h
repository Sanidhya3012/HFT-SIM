#pragma once
#include <string>
#include <ctime>

inline std::string formatTimestamp(std::uint64_t ms_since_epoch) {
    std::time_t t = ms_since_epoch / 1000;
    std::tm* tm_ptr = std::localtime(&t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%H:%M:%S-%d/%m/%Y", tm_ptr);
    return std::string(buf);
} 