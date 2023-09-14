cmake_minimum_required(VERSION 3.26)
# 'Google_test' is the subproject name
project(Google_tests)
# ignore if google tests aren't installed
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/")


    # 'lib' is the folder with Google Test sources
    add_subdirectory(lib)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    # 'Google_Tests_run' is the target name
    # 'test1.cpp test2.cpp' are source files with tests
    add_executable(Google_Tests_run test_unittest.cpp)
    target_link_libraries(Google_Tests_run gtest gtest_main)
endif ()