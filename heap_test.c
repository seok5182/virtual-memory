#include <stdio.h>
#include <stdlib.h> // malloc, free를 위해 필요
#include <unistd.h> // getpid를 위해 필요
#include <string.h> // memset을 위해 필요

int main() {
    int pid = getpid();
    printf("현재 프로세스 ID(PID) : %d\n", pid);
    printf("------------------------------------------\n");

    // 10MB 크기 설정(10 * 1024 * 1024 bytes)
    size_t size = 10 * 1024 * 1024;

    // 1. 힙 영역 할당(예약)
    char *ptr = (char *)malloc(size);

    if (ptr == NULL) {
        printf("메모리 할당 실패!\n");
        return 1;
    }

    printf("할당된 가상 주소 : %p\n", (void*)ptr);
    printf("1단계 : 할당만 한 상태입니다. 엔터를 치면 데이터를 씁니다.\n");
    printf("(지금 다른 터미널에서 'vmmap %d'를 확인해 보세요!)\n", pid);
    getchar();

    // 2. 실제 데이터 쓰기(접근)
    // 이 시점에 비로소 Page Fault가 발생하며 실제 RAM이 할당됩니다.
    memset(ptr, 'A', size);

    printf("2단계 : 10MB 데이터를 모두 썼습니다. 엔터를 치면 종료됩니다.\n");
    printf("(다시 'vmmap %d'를 확인하면 DIRTY 수치가 올라가 있을 겁니다!)\n", pid);
    getchar();

    // 3. 해제
    free(ptr);

    return 0;
}