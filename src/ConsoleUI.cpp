#include "ConsoleUI.h"
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#endif

void EnableVTMode() {
#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return; // Bug fix: Validate handle
    
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void DrawProgressBar(const std::string& label, double percentage, int width) {
    // Bound the percentage to prevent buffer logic errors
    if (percentage < 0.0) percentage = 0.0;
    if (percentage > 100.0) percentage = 100.0;

    int filled = static_cast<int>((percentage / 100.0) * width);
    
    // ANSI color code: Green (<70%), Yellow (70-85%), Red (>=85%)
    std::string color = "\x1B[32m"; 
    if (percentage >= 85.0) color = "\x1B[31m";
    else if (percentage >= 70.0) color = "\x1B[33m";

    std::cout << label << " [";
    std::cout << color;
    for (int i = 0; i < width; ++i) {
        if (i < filled) std::cout << "=";
        else std::cout << " ";
    }
    std::cout << "\x1B[0m] " << percentage << "%\n"; // \x1B[0m resets the color formatting
}