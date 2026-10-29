#include "MemorySampler.h"

#if defined(_WIN32)
#include <windows.h>
#endif

MemoryStats GetMemoryMetrics() {
#if defined(_WIN32)
    MEMORYSTATUSEX memStatus{};
    memStatus.dwLength = sizeof(memStatus);

    if (GlobalMemoryStatusEx(&memStatus)) {
        return {
            static_cast<std::uint32_t>(memStatus.dwMemoryLoad),
            static_cast<std::uint64_t>(memStatus.ullTotalPhys / (1024ULL * 1024ULL)),
            static_cast<std::uint64_t>(memStatus.ullAvailPhys / (1024ULL * 1024ULL))
        };
    }
#endif
    return { 0, 0, 0 }; // Safe fallback for non-Windows systems or API failure
}