cmake_minimum_required(VERSION 3.5)
project(multicast)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES receiver.c sender.c)
add_executable(SENDER sender.c)
add_executable(RECEIVER receiver.c)