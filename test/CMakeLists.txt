project(Google_tests)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run ${TEST_SRC_FILES})
target_link_libraries(Google_Tests_run gtest gtest_main ${CMAKE_PROJECT_NAME}_lib)
target_link_libraries(Google_Tests_run yaml-cpp)
