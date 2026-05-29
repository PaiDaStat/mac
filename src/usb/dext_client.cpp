#include "dext_client.h"

#include "usb_parser.h"

#include <chrono>
#include <random>

DextClient::DextClient() = default;

DextClient::~DextClient() {
    stop_capture();
}

void DextClient::start_capture() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) {
        return;
    }

    worker_ = std::thread(&DextClient::worker_loop, this);
}

void DextClient::stop_capture() {
    bool expected = true;
    if (!running_.compare_exchange_strong(expected, false)) {
        return;
    }

    if (worker_.joinable()) {
        worker_.join();
    }
}

bool DextClient::is_running() const {
    return running_.load(std::memory_order_relaxed);
}

uint64_t DextClient::dropped_packets() const {
    return queue_.dropped();
}

bool DextClient::poll(Packet& packet) {
    auto result = queue_.pop();

    if (!result.has_value()) {
        return false;
    }

    packet = *result;
    return true;
}

void DextClient::worker_loop() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> byte_dist(0, 255);
    std::uniform_int_distribution<int> endpoint_dist(1, 4);
    std::uniform_int_distribution<int> length_dist(8, 64);

    while (running_.load(std::memory_order_relaxed)) {
        Packet p;

        p.seq = ++seq_;
        p.timestamp_sec = std::chrono::duration<double>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        p.bus = 1;
        p.addr = 2;
        p.ep = static_cast<uint8_t>(endpoint_dist(rng));
        p.xfer_type = 3;
        p.device_name = "USB Keyboard";
        p.summary = "HID Interrupt IN";
        p.raw.resize(static_cast<std::size_t>(length_dist(rng)));

        for (auto& b : p.raw) {
            b = static_cast<uint8_t>(byte_dist(rng));
        }

        usb::parse_packet(p);
        queue_.push(p);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
