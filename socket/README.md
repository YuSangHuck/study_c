- [socket](#socket)
  - [simple test](#simple-test)
  - [blk vs nonb](#blk-vs-nonb)
  - [블로킹,논블로킹 소켓이란](#블로킹논블로킹-소켓이란)
  - [블로킹 및 논블로킹 설정](#블로킹-및-논블로킹-설정)
  - [설정 조합별 동작 방식](#설정-조합별-동작-방식)
  - [코드상 처리](#코드상-처리)


# socket

> blk는 block
> nonb는 non-block
> **fd에 대해서 send,read하면, 커널 내부에 tcpbuf에 대해서 뭔갈 한다** 라고 가정.

## simple test

```bash
# terminal 1
docker-compose up --build
# terminal 2
echo "Hello Blocking Server" | nc localhost 8081
echo "Hello Non-Blocking Server" | nc localhost 8082
```

## blk vs nonb


- 사용법
  - 결국 socket이나, send,recv에 flag에 O_NONBLOCK을 쓰면 되는거고
  - 4가지 경우의 수는 어떻게 되는지는 찾아봐야 됨
  | socket nonb 여부 |     | op  | result |
  | ---------------- | --- | --- | ------ |
  | O                |     |     |        |
  - **socket fd 설정을 어떻게했더라?**

---

## 블로킹,논블로킹 소켓이란

accept(), connect(), recv(), send() 에 대해서

- blocking: 완료될때까지 반환되지 않음
- non-blocking: 즉시 반환됨

의 차이가 있다

- blk
  - send도 대기, recv도 대기
  - send는 tcpbuf에 다 쌓을때까지 대기
  - 쌓던도중 에러면 바로 리턴
  - recv도 tcpbuf에서 다 읽어올떄까지 대기
  - **읽던도중 에러면 바로 리턴? 이럴일이 있나?**
- nonb
  - send,recv하고 할거하고 바로 빠져나옴(대기 없음)
- socket에서 block, non-block은 뭘 의미하는가?

## 블로킹 및 논블로킹 설정

- 소켓 자체를 논블로킹 모드로 설정

```c
#include <fcntl.h>

int flags = fcntl(socket_fd, F_GETFL, 0);
fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
```

- send 및 recv 함수에 논블로킹 플래그 설정

```c
// Non-blocking send
ssize_t sent_bytes = send(socket_fd, buffer, length, MSG_DONTWAIT);

// Non-blocking recv
ssize_t received_bytes = recv(socket_fd, buffer, length, MSG_DONTWAIT);
```


## 설정 조합별 동작 방식

- 총 16가지임([클라 or 서버 / block소켓 or non-block소켓] / [클라 or 서버 / block send or non-block send])
  - send,recv가 non-block이 최우선순위(소켓이 뭐든간에 non-block으로 처리)
- non-block 여부만 표기(o: non-block, x: block)

| 소켓 설정 \ 플래그 설정 | 서버 x / 클라이언트 x | 서버 x / 클라이언트 o | 서버 o / 클라이언트 x | 서버 o / 클라이언트 o |
| ----------------------- | --------------------- | --------------------- | --------------------- | --------------------- |
| 서버 x - 클라이언트 x   | 서버 x / 클라이언트 x | 서버 x / 클라이언트 o | 서버 o / 클라이언트 x | 서버 o / 클라이언트 o |
| 서버 x - 클라이언트 o   | 서버 x - 클라이언트 o | 서버 x - 클라이언트 o | 서버 o - 클라이언트 o | 서버 o - 클라이언트 o |
| 서버 o - 클라이언트 x   | 서버 o - 클라이언트 x | 서버 o - 클라이언트 o | 서버 o - 클라이언트 x | 서버 o / 클라이언트 o |
| 서버 o - 클라이언트 o   | 서버 o - 클라이언트 o | 서버 o - 클라이언트 o | 서버 o - 클라이언트 o | 서버 o - 클라이언트 o |

## 코드상 처리

- conn_msg, sock_msg, sock_ex로 나뉘고
- nonb면 EWOULDBLOCK 일때는 위로 올려서 conn_msg에서 내부에서 처리한다
  - wakeup_evt event를 만들고, EWOULDBLOCK면 큐에 달아놓는다
- blk면 걍 return 해버리고 EWOULDBLOCK 일때는 fd에 대해서 wait한다
