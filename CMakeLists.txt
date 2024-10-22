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

# Fetch Asio library
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2  # Use the desired version tag
)

# Ensure Asio is built as a standalone library
set(ASIO_BUILD_EXAMPLES OFF)
set(ASIO_BUILD_TESTS OFF)
set(ASIO_INSTALL OFF)

# Set ASIO_INCLUDE_DIR to point to Asio's include directory
set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include")

FetchContent_MakeAvailable(asio)

# Fetch Crow library
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.0
)

FetchContent_MakeAvailable(crow)

# Fetch nlohmann/json library for JSON handling
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

FetchContent_MakeAvailable( cryptopp-cmake_wrapper)

# Add subdirectory for source files (your existing source code)
add_subdirectory(src)

# Swagger UI Integration (move it after defining the project target)
include(ExternalProject)

ExternalProject_Add(
    swagger_ui
    URL https://github.com/swagger-api/swagger-ui/archive/refs/tags/v4.19.0.zip
    PREFIX ${CMAKE_BINARY_DIR}/swagger_ui
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Set the path where Swagger UI will be extracted
set(SWAGGER_UI_DIR "${CMAKE_BINARY_DIR}/swagger_ui/src/swagger_ui/swagger-ui-4.19.0/dist")

# Ensure Swagger UI is downloaded before the project is built
add_custom_target(
    prepare_swagger_ui
    DEPENDS swagger_ui
)

# Link the swagger-ui path in your app's code (C++ part)
add_definitions(-DSWAGGER_UI_PATH="${SWAGGER_UI_DIR}")

# Add the dependency on prepare_swagger_ui to ensure Swagger UI is ready
add_dependencies(${projectName} prepare_swagger_ui)