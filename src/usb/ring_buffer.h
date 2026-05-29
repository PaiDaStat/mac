#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <optional>

template<typename T, std::size_t Size>
class SPSCRingBuffer {
public:
    static_assert(Size > 1, "SPSC ring buffer size must be greater than 1");

    bool push(const T& item) {
        const auto head = head_.load(std::memory_order_relaxed);
        const auto next = (head + 1) % Size;

        if (next == tail_.load(std::memory_order_acquire)) {
            dropped_.fetch_add(1, std::memory_order_relaxed);
            return false;
        }

        buffer_[head] = item;
        head_.store(next, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() {
        const auto tail = tail_.load(std::memory_order_relaxed);

        if (tail == head_.load(std::memory_order_acquire)) {
            return {};
        }

        T item = buffer_[tail];
        tail_.store((tail + 1) % Size, std::memory_order_release);
        return item;
    }

    uint64_t dropped() const {
        return dropped_.load(std::memory_order_relaxed);
    }

private:
    std::array<T, Size> buffer_{};
    std::atomic<std::size_t> head_ = 0;
    std::atomic<std::size_t> tail_ = 0;
    std::atomic<uint64_t> dropped_ = 0;
};
