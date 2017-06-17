cmake_minimum_required(VERSION 2.8.12)
project(ising2_mc CXX)

#For efficiency, compile with optimization by default
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

add_executable(${PROJECT_NAME} main.cpp ising.cpp)

# Request the ALPSCore package (with all components)
# The ALPSCore package must be installed in some
# standard place (like /usr/local),
# or somewhere in your PATH. 
# Otherwise, point ALPSCore_DIR environment variable
# to the ALPScore installation tree.
find_package(ALPSCore REQUIRED)

# Use ALPSCore_LIBRARIES variable to link to ALPSCore 
target_link_libraries(${PROJECT_NAME} ${ALPSCore_LIBRARIES})
