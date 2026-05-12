find_package(GTest REQUIRED)
file(GLOB_RECURSE TEST_SOURCES "*test*")

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests Algo GTest::GTest GTest::Main headers)

add_test(NAME HandlersTest COMMAND run_tests)