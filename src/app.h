#pragma once

#include "usb/dext_client.h"
#include "usb/packet.h"

#include <deque>
#include <string>

class App {
public:
    void initialize();
    void shutdown();
    void update();

    void start_capture();
    void stop_capture();
    void clear_packets();

    bool export_pcap(const std::string& path) const;
    bool is_capturing() const;
    uint64_t dropped_packets() const;

    std::deque<Packet>& packets();
    const std::deque<Packet>& packets() const;

    int selected_index = -1;

private:
    DextClient dext_;
    std::deque<Packet> packets_;
};
