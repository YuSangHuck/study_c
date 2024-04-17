cmake_minimum_required(VERSION 3.0)
project(socket_client)

set(CMAKE_C_STANDARD 11)

# 클라이언트 소스 코드
set(CLIENT_SOURCE_FILES client.c)

# 클라이언트 바이너리 생성
add_executable(client_exe ${CLIENT_SOURCE_FILES})
