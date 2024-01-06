cmake_minimum_required(VERSION 3.14) 
project(RPAL_INT_TEST) 

find_package(GTest REQUIRED)
include(GoogleTest)  

include_directories(${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

file(GLOB TEST_SOURCES "*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} GTest::GTest GTest::Main rpal_interpreter_lib)
    if (UNIX)
        target_link_libraries(${TEST_NAME} pthread)
    endif()
    gtest_discover_tests(${TEST_NAME})
endforeach()
 