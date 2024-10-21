cmake_minimum_required(VERSION 3.30.3)

# Set project name variable
set(projectName HorovyiBlockchain)

# Set project name
project(${projectName} LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Include cryptopp-cmake
add_subdirectory(external/cryptopp-cmake)

# Add subdirectories
add_subdirectory(src)
