cmake_minimum_required(VERSION 3.20)
project(SpartanGoldcpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenSSL REQUIRED)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)

add_library(spartan_gold_lib
    src/transaction.cpp
    src/utils.cpp
    src/block.cpp
    src/blockchain.cpp
    src/fake-net.cpp
    src/client.cpp
    src/miner.cpp
)

target_include_directories(spartan_gold_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(spartan_gold_lib
    PUBLIC
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
)

add_executable(spartan_gold_app
    src/main.cpp
)

target_link_libraries(spartan_gold_app
    PRIVATE
        spartan_gold_lib
)