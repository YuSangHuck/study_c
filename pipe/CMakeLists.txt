cmake_minimum_required(VERSION 3.10)
project(thread_communication_example C)

set(CMAKE_C_STANDARD 11)

add_executable(thread_communication_example main.c)

target_link_libraries(thread_communication_example pthread)
