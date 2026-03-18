#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE (10 * 1024 * 1024) // 10MB

int main() {
    // 1단계: 부모가 10MB 할당 후 'A'로 채우기
    char *data = (char *)malloc(SIZE);
    memset(data, 'A', SIZE);

    printf("=== [1단계] fork() 전 ===\n");
    printf("[부모] PID: %d\n", getpid());
    printf("[부모] data[0] = %c\n", data[0]);
    printf("[부모] vmmap %d 으로 확인하세요.\n", getpid());
    printf("[부모] Enter를 누르면 fork() 실행...\n");
    getchar();

    // 2단계: fork() - 프로세스 복제
    pid_t pid = fork();

    if (pid == 0) {
        // ── 자식 프로세스 ──
        printf("\n=== [2단계] fork() 직후 (자식, 쓰기 전) ===\n");
        printf("[자식] PID: %d\n", getpid());
        printf("[자식] data[0] = %c (부모 데이터 그대로 보임)\n", data[0]);
        printf("[자식] vmmap %d 으로 확인하세요. (DIRTY가 늘지 않았는지 확인)\n", getpid());
        printf("[자식] Enter를 누르면 데이터 수정 (CoW 발동)...\n");
        getchar();

        // 3단계: 자식이 데이터 수정 → CoW 발동
        memset(data, 'B', SIZE);

        printf("\n=== [3단계] 자식 쓰기 완료 (CoW 발동 후) ===\n");
        printf("[자식] data[0] = %c (수정됨)\n", data[0]);
        printf("[자식] vmmap %d 으로 확인하세요. (DIRTY 10MB 증가했는지 확인)\n", getpid());
        printf("[자식] Enter를 누르면 종료...\n");
        getchar();

        free(data);
        exit(0);

    } else {
        // ── 부모 프로세스 ──
        printf("\n[부모] fork() 완료. 자식 PID: %d\n", pid);
        printf("[부모] 자식이 끝날 때까지 대기 중...\n");

        wait(NULL); // 자식 종료 대기

        // 4단계: 자식이 수정했어도 부모 데이터는 그대로
        printf("\n=== [4단계] 자식 종료 후 부모 확인 ===\n");
        printf("[부모] data[0] = %c (자식이 'B'로 바꿨어도 부모는 'A' 그대로)\n", data[0]);
        printf("[부모] → 프로세스 격리 확인 완료\n");

        free(data);
    }

    return 0;
}
