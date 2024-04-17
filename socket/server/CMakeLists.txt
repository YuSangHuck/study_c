cmake_minimum_required(VERSION 3.0)
project(socket_server)

set(CMAKE_C_STANDARD 11)

# 서버 소스 코드
set(SERVER_SOURCE_FILES server.c)

# 서버 바이너리 생성
add_executable(server_exe ${SERVER_SOURCE_FILES})
