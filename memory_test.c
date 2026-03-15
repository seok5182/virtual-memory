#include <stdio.h>
#include <unistd.h>

int global_var = 100; // 데이터 영역에 저장될 전역 변수

int main() {
    int local_var = 10; // 스택 영역에 저장될 지역 변수

    printf("현재 프로세스 ID (PID): %d\n", getpid());
    printf("전역 변수(global_var)의 가상 주소: %p\n", (void*)&global_var);
    printf("지역 변수(local_var)의 가상 주소: %p\n", (void*)&local_var);

    // 프로그램이 종료되지 않게 대기 (주소를 확인하기 위함)
    printf("엔터를 치면 종료됩니다...\n");
    getchar();

    return 0;
}