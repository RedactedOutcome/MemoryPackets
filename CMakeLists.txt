cmake_minimum_required(VERSION 3.19.0)
project(MemoryPackets)

#Include this file as a subdirectory
#eg add_subdirectory(...)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")