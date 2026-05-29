#include "pcap_exporter.h"

#include <cstdint>
#include <fstream>

namespace exporter {

#pragma pack(push, 1)

struct PcapGlobalHeader {
    uint32_t magic = 0xa1b2c3d4;
    uint16_t major = 2;
    uint16_t minor = 4;
    int32_t zone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 220;
};

struct PcapPacketHeader {
    uint32_t ts_sec = 0;
    uint32_t ts_usec = 0;
    uint32_t incl_len = 0;
    uint32_t orig_len = 0;
};

#pragma pack(pop)

bool export_pcap(const std::string& path, const std::deque<Packet>& packets) {
    std::ofstream out(path, std::ios::binary);

    if (!out.is_open()) {
        return false;
    }

    PcapGlobalHeader gh;
    out.write(reinterpret_cast<const char*>(&gh), sizeof(gh));

    for (const auto& p : packets) {
        PcapPacketHeader ph;
        ph.ts_sec = static_cast<uint32_t>(p.timestamp_sec);
        ph.ts_usec = static_cast<uint32_t>((p.timestamp_sec - ph.ts_sec) * 1000000.0);
        ph.incl_len = static_cast<uint32_t>(p.raw.size());
        ph.orig_len = static_cast<uint32_t>(p.raw.size());

        out.write(reinterpret_cast<const char*>(&ph), sizeof(ph));
        if (!p.raw.empty()) {
            out.write(reinterpret_cast<const char*>(p.raw.data()),
                static_cast<std::streamsize>(p.raw.size()));
        }
    }

    return out.good();
}

}
