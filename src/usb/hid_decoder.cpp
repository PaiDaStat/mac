#include "hid_decoder.h"

#include <array>
#include <cstddef>
#include <sstream>

namespace {

const char* key_name(uint8_t usage) {
    static constexpr std::array<const char*, 26> letters = {
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
    };

    if (usage >= 0x04 && usage <= 0x1d) {
        return letters[usage - 0x04];
    }

    if (usage >= 0x1e && usage <= 0x27) {
        static constexpr std::array<const char*, 10> digits = {
            "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
        };
        return digits[usage - 0x1e];
    }

    return nullptr;
}

}

namespace hid {

std::string decode(const Packet& packet) {
    if (packet.raw.empty()) {
        return "Empty report";
    }

    if (packet.raw.size() < 8) {
        return "Short HID report";
    }

    std::ostringstream out;
    out << "Keyboard Report";

    bool has_key = false;
    for (std::size_t i = 2; i < packet.raw.size(); ++i) {
        const auto usage = packet.raw[i];
        if (usage == 0) {
            continue;
        }

        if (!has_key) {
            out << " keys:";
            has_key = true;
        }

        if (const char* name = key_name(usage)) {
            out << ' ' << name;
        } else {
            out << " 0x" << std::hex << static_cast<int>(usage) << std::dec;
        }
    }

    return out.str();
}

}
