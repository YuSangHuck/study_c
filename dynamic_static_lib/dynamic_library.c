#include <stdio.h>

// 전역 변수 선언
int global_dynamic_variable = 10;

// 동적 라이브러리에서 호출할 함수 정의
void print_dynamic_hello() {
    printf("Dynamic Library says: Hello! Global variable value: %d\n", global_dynamic_variable);
}

// 전역 변수를 반환하는 함수 정의
int* get_global_dynamic_variable() {
    return &global_dynamic_variable;
}