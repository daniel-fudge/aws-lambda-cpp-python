file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${BINARY}_run "main.cpp")

target_link_libraries(${BINARY}_run yaml-cpp)
if (${CLOUD9})
  target_link_libraries(${BINARY}_run ${AWSSDK_LINK_LIBRARIES})
  target_compile_options(${BINARY}_run PRIVATE "-Wall" "-Wextra")
endif()

add_library(${BINARY}_lib STATIC ${SRC_FILES} ${HEADER_FILES})
