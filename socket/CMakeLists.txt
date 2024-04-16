cmake_minimum_required(VERSION 3.0)
project(socket_programming)

# 클라이언트와 서버를 동시에 빌드하기 위해 하위 디렉토리로 이동
add_subdirectory(client)
add_subdirectory(server)
