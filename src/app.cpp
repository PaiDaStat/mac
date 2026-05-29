#include "app.h"

#include "utils/pcap_exporter.h"

void App::initialize() {
    dext_.start_capture();
}

void App::shutdown() {
    dext_.stop_capture();
}

void App::update() {
    constexpr int max_per_frame = 100;

    for (int i = 0; i < max_per_frame; ++i) {
        Packet packet;

        if (!dext_.poll(packet)) {
            break;
        }

        packets_.push_back(packet);

        if (packets_.size() > 100000) {
            packets_.pop_front();
            if (selected_index > 0) {
                --selected_index;
            }
        }
    }
}

void App::start_capture() {
    dext_.start_capture();
}

void App::stop_capture() {
    dext_.stop_capture();
}

void App::clear_packets() {
    packets_.clear();
    selected_index = -1;
}

bool App::export_pcap(const std::string& path) const {
    return exporter::export_pcap(path, packets_);
}

bool App::is_capturing() const {
    return dext_.is_running();
}

uint64_t App::dropped_packets() const {
    return dext_.dropped_packets();
}

std::deque<Packet>& App::packets() {
    return packets_;
}

const std::deque<Packet>& App::packets() const {
    return packets_;
}
