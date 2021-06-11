cmake_minimum_required(VERSION 3.0) 
project(cats LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(
        ${CURSES_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRC
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

add_executable(cats ${all_SRC})
target_link_libraries(cats ${CURSES_LIBRARIES})
target_link_libraries(cats ${CMAKE_THREAD_LIBS_INIT})