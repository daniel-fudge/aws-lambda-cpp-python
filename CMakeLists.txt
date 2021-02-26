cmake_minimum_required(VERSION 3.17)
project(cpp-read-yaml)

# https://github.com/alicevision/geogram/issues/2#issuecomment-536835320
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 14)
set(BINARY ${CMAKE_PROJECT_NAME})
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")

set(CLOUD9 ON)
if(${CLOUD9})
  # Add the AWS SDK package if building on Cloud9
  find_package(AWSSDK REQUIRED COMPONENTS s3)
else()
  # Add the test directory if developing locally with full GoogleTest integration
  add_subdirectory(test)
endif()

include_directories(include)
include_directories(third-party/yaml-cpp/include)

add_subdirectory(third-party/yaml-cpp/ yaml-cpp)
add_subdirectory(src)
