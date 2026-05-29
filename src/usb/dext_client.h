#pragma once

#include "packet.h"
#include "ring_buffer.h"

#include <atomic>
#include <thread>

class DextClient {
public:
    DextClient();
    ~DextClient();

    DextClient(const DextClient&) = delete;
    DextClient& operator=(const DextClient&) = delete;

    void start_capture();
    void stop_capture();
    bool is_running() const;
    uint64_t dropped_packets() const;

    bool poll(Packet& packet);

private:
    void worker_loop();

    std::thread worker_;
    std::atomic<bool> running_ = false;
    SPSCRingBuffer<Packet, 8192> queue_;
    uint64_t seq_ = 0;
};
