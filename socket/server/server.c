#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE (1024 * 1024) // 1MB

int main() {
    // 소켓 생성
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(12345);

    // 바인딩
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    // 수신 대기
    listen(server_socket, 5);

    // 클라이언트 연결 수락
    int client_socket = accept(server_socket, NULL, NULL);

    // 주기적으로 수신 버퍼에 쌓인 데이터 크기 출력
    while (1) {
        int buffer_size;
        ioctl(client_socket, FIONREAD, &buffer_size); // 수신 버퍼에 쌓인 데이터 크기 확인
        printf("Received data size in buffer: %d bytes\n", buffer_size);
        sleep(1); // 1초 대기
    }

    // 소켓 닫기
    close(client_socket);
    close(server_socket);

    return 0;
}
