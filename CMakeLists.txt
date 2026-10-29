cmake_minimum_required(VERSION 3.20)
project(WinResourceMonitor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h")

add_executable(WinResourceMonitor ${SOURCES} ${HEADERS})

# Conditionally link Windows-specific libraries only on Windows
if (WIN32)
    target_link_libraries(WinResourceMonitor PRIVATE pdh)
endif()