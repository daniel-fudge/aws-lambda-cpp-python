cmake_minimum_required(VERSION 3.17)
project(cpp-read-yaml)

set(CMAKE_CXX_STANDARD 14)
set(BINARY ${CMAKE_PROJECT_NAME})
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")

# find_package(AWSSDK COMPONENTS core)

include_directories(include)
include_directories(third-party/yaml-cpp/include)

add_subdirectory(third-party/yaml-cpp/ yaml-cpp)
add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_CXX_EXTENSIONS ON)