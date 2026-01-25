cmake_minimum_required(VERSION 3.10)
project(SOUI)

add_library(SOUI INTERFACE)

target_include_directories(SOUI INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)