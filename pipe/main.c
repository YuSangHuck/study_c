#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main_to_child_pipe[2];
int child_to_main_pipe[2];

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

int main() {
    pthread_t child_thread;
    char message[] = "ping";
    char response_buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // 파이프 생성
    if (pipe(main_to_child_pipe) == -1 || pipe(child_to_main_pipe) == -1) {
        perror("pipe");
        return 1;
    }

    // 자식 스레드 생성
    if (pthread_create(&child_thread, NULL, child_thread_function, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    printf("Main to child pipe: read_fd=%d, write_fd=%d\n", main_to_child_pipe[0], main_to_child_pipe[1]);
    printf("Child to main pipe: read_fd=%d, write_fd=%d\n", child_to_main_pipe[0], child_to_main_pipe[1]);

    // close(main_to_child_pipe[0]); // 메인 스레드는 자식 스레드로부터 읽기를 하지 않으므로 해당 파이프 닫음
    // close(child_to_main_pipe[1]); // 메인 스레드는 자식 스레드로 쓰기를 하므로 해당 파이프 닫지 않음

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

    // close(main_to_child_pipe[1]);
    // close(child_to_main_pipe[0]);

    pthread_join(child_thread, NULL);

    return 0;
}
