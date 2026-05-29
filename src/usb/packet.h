#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Packet {
    uint64_t seq = 0;
    double timestamp_sec = 0.0;
    uint16_t bus = 0;
    uint16_t addr = 0;
    uint8_t ep = 0;
    uint8_t xfer_type = 0;

    std::string device_name;
    std::string summary;
    std::vector<uint8_t> raw;
};
