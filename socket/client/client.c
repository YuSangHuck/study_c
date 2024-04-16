#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE (1024 * 1024) // 1MB

int main() {
    // 소켓 생성
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(12345);

    // 서버에 연결
    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    // 데이터 전송
    char* message = (char*)malloc(BUFFER_SIZE);
    memset(message, 'A', BUFFER_SIZE); // 1MB 크기의 데이터 생성

    while (1) {
        write(client_socket, message, BUFFER_SIZE);
        sleep(1); // 1초 대기
    }

    // 메모리 해제 및 소켓 닫기
    free(message);
    close(client_socket);

    return 0;
}
