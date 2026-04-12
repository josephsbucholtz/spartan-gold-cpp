cmake_minimum_required(VERSION 3.20)
project(SpartanGoldcpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenSSL REQUIRED COMPONENTS Crypto)

add_library(spartan_gold_lib
    src/utils.cpp
    src/transaction.cpp
    src/block.cpp
    src/client.cpp
    src/network.cpp
    src/miner.cpp
    src/blockchain.cpp
)

target_include_directories(spartan_gold_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(spartan_gold_lib
    PUBLIC
        OpenSSL::Crypto
)

add_executable(spartan_gold_app
    src/main.cpp
)

target_link_libraries(spartan_gold_app
    PRIVATE
        spartan_gold_lib
)