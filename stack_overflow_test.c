#include <stdio.h>
#include <unistd.h>

// 재귀 호출 횟수를 추적
void recursive(int depth) {
    // 스택 프레임을 키우기 위해 지역 변수 선언
    char buffer[1024]; // 1KB짜리 지역 변수
    buffer[0] = depth; // 최적화로 제거되지 않도록 실제로 사용

    printf("depth : %d | 스택 주소 : %p\n", depth, (void*)buffer);

    recursive(depth + 1);
}

int main() {
    printf("PID : %d\n", getpid());
    printf("실험 전 vmmap으로 스택 크기 확인하세요.\n");
    printf("엔터를 치면 재귀 시작...\n");
    getchar();

    recursive(0);

    return 0;
}