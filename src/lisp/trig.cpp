#include <array>
#include <cmath>
#include <cstdint>

std::array<int32_t, 360> generateSinTable() {
    std::array<int32_t, 360> table;
    for (int i = 0; i < 360; ++i) {
        table[i] = static_cast<int32_t>(std::round(65536 * std::sin(M_PI * i / 180.0)));
    }
    return table;
}

std::array<uint16_t, 1662> generateAtanTable() {
    std::array<uint16_t, 1662> table;
    for (int i = 0; i < 1662; ++i) {
        table[i] = static_cast<uint16_t>(std::max(0.0, std::round(std::atan(i / 29.0) / M_PI * 180.0) - 45.0));
    }
    return table;
}

auto sin_table = generateSinTable();
auto atan_table = generateAtanTable();