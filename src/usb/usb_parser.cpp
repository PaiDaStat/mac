#include "usb_parser.h"

#include "hid_decoder.h"

namespace usb {

const char* transfer_type_name(uint8_t xfer_type) {
    switch (xfer_type) {
    case 0:
        return "CTRL";
    case 1:
        return "ISO";
    case 2:
        return "BULK";
    case 3:
        return "INT";
    default:
        return "UNK";
    }
}

void parse_packet(Packet& packet) {
    if (packet.xfer_type == 3) {
        packet.summary += " | ";
        packet.summary += hid::decode(packet);
        return;
    }

    packet.summary += " | Parsed";
}

}
