cmake_minimum_required(VERSION 3.11)

set(ENV{CXX} "g++")
project(MINIMUMMULTICUT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "C++ standard ${CMAKE_CXX_STANDARD}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address")

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)


add_executable(main main.cpp)
target_link_libraries(main Algo headers)

set_target_properties(main PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

message(STATUS "Build completed!")

