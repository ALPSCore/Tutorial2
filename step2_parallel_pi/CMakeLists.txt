cmake_minimum_required(VERSION 2.8.12)
project(Alpsdemo)

# we need ALPSCore
find_package(ALPSCore REQUIRED)

# Executable `alpsdemo` is made from
# two source files: `main.cpp` and `simulation.cpp`
add_executable(alpsdemo main.cpp simulation.cpp)

# Executable needs libraries from ALPSCore
target_link_libraries(alpsdemo ${ALPSCore_LIBRARIES})
