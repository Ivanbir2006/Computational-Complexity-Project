cmake_minimum_required(VERSION 3.11)

set(ENV{CXX} "g++")
project(MINIMUMMULTICUT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "C++ standard ${CMAKE_CXX_STANDARD}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address")

add_executable(main 
    main.cpp
    source/Algo.cpp
)

target_include_directories(main PRIVATE source)
message(STATUS "Build completed!")
