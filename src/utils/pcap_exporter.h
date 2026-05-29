#pragma once

#include "../usb/packet.h"

#include <deque>
#include <string>

namespace exporter {
    bool export_pcap(const std::string& path, const std::deque<Packet>& packets);
}
