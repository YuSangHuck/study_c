#include <stdio.h>

// 전역 변수 선언
int global_static_variable = 90;

// 정적 라이브러리에서 호출할 함수 정의
void print_static_hello() {
    printf("Static Library says: Hello! Global variable value: %d\n", global_static_variable);
}

// 전역 변수를 반환하는 함수 정의
int* get_global_static_variable() {
    return &global_static_variable;
}