#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

int main_to_child_pipe[2];
int child_to_main_pipe[2];
int limited_fd[2];

void *child_thread_function(void *arg) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while (1) {
        // 메인 스레드로부터 "ping" 메시지 받기
        bytes_read = read(main_to_child_pipe[0], buffer, BUFFER_SIZE);
        printf("Child thread received message from main (read_fd=%d): %s\n", main_to_child_pipe[0], buffer);

        // "pong" 메시지 보내기
        char response[] = "pong";
        write(child_to_main_pipe[1], response, sizeof(response));
        printf("Child thread sent message to main (write_fd=%d): %s\n", child_to_main_pipe[1], response);
    }

    close(main_to_child_pipe[0]);
    close(child_to_main_pipe[1]);

    pthread_exit(NULL);
}

void *no_read_thread_function(void *arg) {
    ssize_t pipe_size;

    while (1) {
        // limited_fd 파이프의 크기를 가져옴
        pipe_size = fcntl(limited_fd[0], F_GETPIPE_SZ);
        printf("Size of data in limited_fd: %zd bytes\n", pipe_size);
        sleep(2); // 2초마다 크기 출력
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t child_thread, no_read_thread;
    char message[] = "ping";
    char response_buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // 파이프 생성
    if (pipe(main_to_child_pipe) == -1 || pipe(child_to_main_pipe) == -1 || pipe(limited_fd) == -1) {
        perror("pipe");
        return 1;
    }

    // limited_fd 파이프의 크기 제한 설정
    if (fcntl(limited_fd[0], F_SETPIPE_SZ, 10240) == -1) { // 10KB로 설정
        perror("fcntl");
        return 1;
    }

    // 자식 스레드 생성
    if (pthread_create(&child_thread, NULL, child_thread_function, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    printf("Main to child pipe: read_fd=%d, write_fd=%d\n", main_to_child_pipe[0], main_to_child_pipe[1]);
    printf("Child to main pipe: read_fd=%d, write_fd=%d\n", child_to_main_pipe[0], child_to_main_pipe[1]);

    while (1) {
        // 메인 스레드에서 "ping" 메시지를 자식 스레드로 전송
        ssize_t bytes_written = write(main_to_child_pipe[1], message, sizeof(message));
        if (bytes_written == -1) {
            perror("write");
            return 1; // 에러가 발생하면 프로그램 종료
        }
        printf("Main thread sent message to child (write_fd=%d): %s\n", main_to_child_pipe[1], message);

        // 자식 스레드로부터 "pong" 메시지 받기
        ssize_t bytes_read = read(child_to_main_pipe[0], response_buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            return 1; // 에러가 발생하면 프로그램 종료
        }
        printf("Main thread received response from child (read_fd=%d): %s\n", child_to_main_pipe[0], response_buffer);
        sleep(1); // 1초 대기
    }

    pthread_join(child_thread, NULL);
    pthread_join(no_read_thread, NULL);

    return 0;
}
