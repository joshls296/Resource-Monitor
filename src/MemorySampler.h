#pragma once
#include <cstdint>

struct MemoryStats {
    std::uint32_t memoryLoadPercent;
    std::uint64_t totalPhysMB;
    std::uint64_t availPhysMB;
};

MemoryStats GetMemoryMetrics();