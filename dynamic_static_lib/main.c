#include <stdio.h>

// 정적 라이브러리에서 정의된 함수를 호출하기 위한 선언
extern void print_static_hello();
extern int* get_global_static_variable(); // 전역 변수 값을 가져오기 위한 선언

// 동적 라이브러리에서 정의된 함수를 호출하기 위한 선언
extern void print_dynamic_hello();
extern int* get_global_dynamic_variable(); // 전역 변수 값을 가져오기 위한 선언

int main() {
    int *value;

    // 전역 변수 값을 가져와 출력
    print_dynamic_hello();
    value = get_global_dynamic_variable();
    printf("Global variable value from main: %d\n", *value);
    *value = 20;
    print_dynamic_hello();

    // 전역 변수 값을 가져와 출력
    print_static_hello();
    value = get_global_static_variable();
    printf("Global variable value from main: %d\n", *value);
    *value = 80;
    print_static_hello();


    return 0;
}
