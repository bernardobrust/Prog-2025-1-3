cmake_minimum_required(VERSION 3.30)
project(FileScanner)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -Walloca -Wcast-qual -Wconversion -Wdouble-promotion -Wfloat-equal -Wswitch-default -Wswitch-enum -g")

add_executable(FileScanner src/main.c)

include_directories(lib)

