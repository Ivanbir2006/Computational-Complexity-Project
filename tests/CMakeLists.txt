add_library(Dummy "dummy.cpp")
target_include_directories(Dummy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Dummy headers)

find_package(GTest REQUIRED)
set(TEST_SOURCES
  Dinic_simple_test.cpp
  Complex_test.cpp
  Benchmarks.cpp
  Productivity.cpp
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests Algo Dummy GTest::GTest GTest::Main headers)
add_test(NAME HandlersTest COMMAND run_tests)
