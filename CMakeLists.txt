cmake_minimum_required(VERSION 3.30)
project(FileScanner)

set(CMAKE_C_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(FileScanner src/main.c)

include_directories(lib)

