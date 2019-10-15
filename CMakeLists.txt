cmake_minimum_required(VERSION 3.15)
project(mk1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(wiringPi)

add_executable(untitled main.c hmt_driver.c hmt_driver.h)