cmake_minimum_required(VERSION 3.30.3)

# Set project name variable
set(projectName HorovyiBlockchain)

# Set project name
project(${projectName} LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
    cryptopp-cmake_wrapper
    GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git
    GIT_TAG CRYPTOPP_8_9_0
)

FetchContent_MakeAvailable( cryptopp-cmake_wrapper)

# Add subdirectories
add_subdirectory(src)
