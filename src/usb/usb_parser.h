#pragma once

#include "packet.h"

namespace usb {
    const char* transfer_type_name(uint8_t xfer_type);
    void parse_packet(Packet& packet);
}
