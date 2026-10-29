#include "CpuSampler.h"

CpuSampler::CpuSampler() : cpuQuery(nullptr), cpuCounter(nullptr), isValid(false) {
#if defined(_WIN32)
    if (PdhOpenQueryW(nullptr, 0, &cpuQuery) != ERROR_SUCCESS) return;

    PDH_STATUS status = PdhAddEnglishCounterW(
        cpuQuery,
        L"\\Processor(_Total)\\% Processor Time", 
        0,
        &cpuCounter
    );

    if (status != ERROR_SUCCESS) return;

    isValid = PdhCollectQueryData(cpuQuery) == ERROR_SUCCESS;
#endif
}

CpuSampler::~CpuSampler() {
#if defined(_WIN32)
    if (cpuQuery != nullptr) PdhCloseQuery(cpuQuery);
#endif
}

double CpuSampler::GetCpuUsage() {
#if defined(_WIN32)
    if (!isValid) return 0.0;

    if (PdhCollectQueryData(cpuQuery) != ERROR_SUCCESS) return 0.0;
    PDH_FMT_COUNTERVALUE counterVal;

    if (PdhGetFormattedCounterValue(cpuCounter, PDH_FMT_DOUBLE, nullptr, &counterVal) == ERROR_SUCCESS) {
        return counterVal.doubleValue;
    }
#endif
    return 0.0;
}