#pragma once

#if defined(_WIN32)
#include <windows.h>
#include <pdh.h>
#endif

class CpuSampler {
private:
#if defined(_WIN32)
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuCounter;
#else
    void* cpuQuery;
    void* cpuCounter;
#endif
    bool isValid;

public:
    CpuSampler();
    ~CpuSampler();
    CpuSampler(const CpuSampler&) = delete;
    CpuSampler& operator=(const CpuSampler&) = delete;

    double GetCpuUsage();
};