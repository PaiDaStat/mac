#pragma once

#include "packet.h"

#include <string>

namespace hid {
    std::string decode(const Packet& packet);
}
