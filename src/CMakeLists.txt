# src/CMakeLists.txt
set(APP_SOURCES
    Application.cpp
    Block.cpp
    Blockchain.cpp
    CrowServer.cpp
    main.cpp
    Miner.cpp
    RandomTransactionGenerator.cpp
    Transaction.cpp

)

# Create a library from your application code
add_library(${projectName}_lib ${APP_SOURCES})

# Include directories for the library
target_include_directories(${projectName}_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${asio_SOURCE_DIR}/asio/include
    ${crow_SOURCE_DIR}/include
)

# Link Crypto++ to your library
target_link_libraries(${projectName}_lib PUBLIC 
    cryptopp::cryptopp
    nlohmann_json::nlohmann_json

)

# Check if building on Windows and link Ws2_32 library
if(WIN32)
    target_link_libraries(${projectName}_lib PUBLIC Ws2_32 Mswsock)
endif()

# Create the main executable
add_executable(${projectName} main.cpp)

# Link the application library into the main executable
target_link_libraries(${projectName} PRIVATE ${projectName}_lib)

# Copy users.txt to the build directory after building the executable
add_custom_command(
    TARGET ${projectName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/assets/users.txt
        $<TARGET_FILE_DIR:${projectName}>
)

# Copy swagger.json to the build directory
add_custom_command(
    TARGET ${projectName}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/swagger.json
        ${CMAKE_BINARY_DIR}/swagger.json
    COMMENT "Copying swagger.json to build directory..."
)

