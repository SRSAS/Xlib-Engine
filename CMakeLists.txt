cmake_minimum_required(VERSION 3.29)

project(Xlib_Engine)

project(Xlib_Engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(X11 REQUIRED)

configure_file(EngineConfig.h.in EngineConfig.h)

add_executable(game main.cpp)

target_include_directories(game PUBLIC
    "${PROJECT_BINARY_DIR}"
    )

target_link_libraries(game ${X11_LIBRARIES})
