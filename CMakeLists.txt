cmake_minimum_required(VERSION 3.10)

project(FPSLoopTest VERSION 1.0)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(FPSLoopTest src/main.c)

target_link_libraries(FPSLoopTest PUBLIC fpsloop ${SDL2_LIBRARIES})
target_include_directories(FPSLoopTest PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/FPSLoop"
)
add_library(fpsloop SHARED src/FPSLoop/FPSLoop.c)
