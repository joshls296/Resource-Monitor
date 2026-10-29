#include <iostream>
#include <thread>
#include <chrono>
#include "CpuSampler.h"
#include "MemorySampler.h"
#include "ConsoleUI.h"

int main() {
    EnableVTMode();
    CpuSampler cpu;
    
    // Wait 1 second to allow the PDH counter to establish a baseline delta
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while (true) {
        // ANSI escape sequence: \x1B[2J clears screen, \x1B[H moves cursor to top-left
        std::cout << "\x1B[2J\x1B[H";
        
        MemoryStats mem = GetMemoryMetrics();
        double cpuVal = cpu.GetCpuUsage();

        std::cout << "========================================\n";
        std::cout << "       WINDOWS SYSTEM MONITOR           \n";
        std::cout << "========================================\n\n";

        DrawProgressBar("CPU Usage", cpuVal);
        DrawProgressBar("RAM Usage", static_cast<double>(mem.memoryLoadPercent));

        std::cout << "\nMemory Used: " << (mem.totalPhysMB - mem.availPhysMB) 
                  << " MB / " << mem.totalPhysMB << " MB\n";

        // 1-second refresh rate
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}