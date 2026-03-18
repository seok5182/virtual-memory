#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PAGE_SIZE  (16 * 1024)   // 16KB (macOS ARM64)

#define SMALL_PAGES 512          // 8MB  → TLB 안에 다 들어옴 (~1024개 미만)
#define LARGE_PAGES 16384        // 256MB → TLB 초과

#define SMALL_SIZE (SMALL_PAGES * PAGE_SIZE)
#define LARGE_SIZE (LARGE_PAGES * PAGE_SIZE)

long measure_second_pass(char *arr, int num_pages) {
    volatile long sum = 0;
    struct timespec start, end;

    // 1번 패스: TLB 등록 (워밍업)
    for (int i = 0; i < num_pages; i++) {
        sum += arr[i * PAGE_SIZE];
    }

    // 2번 패스: 시간 측정
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < num_pages; i++) {
        sum += arr[i * PAGE_SIZE];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    // sum을 반환해서 컴파일러 최적화 방지
    // (실제론 시간만 필요하지만 sum이 없으면 루프가 제거될 수 있음)
    (void)sum;

    return (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
}

int main() {
    // 작은 배열 (8MB) - TLB에 다 들어옴
    char *small_arr = (char *)malloc(SMALL_SIZE);
    memset(small_arr, 1, SMALL_SIZE);

    // 큰 배열 (256MB) - TLB 초과
    char *large_arr = (char *)malloc(LARGE_SIZE);
    memset(large_arr, 1, LARGE_SIZE);

    printf("=== TLB Miss 실험 ===\n");
    printf("TLB 추정 용량: ~1024 페이지\n\n");

    long small_ns = measure_second_pass(small_arr, SMALL_PAGES);
    long large_ns = measure_second_pass(large_arr, LARGE_PAGES);

    printf("작은 세트 (%4d 페이지,   8MB) 2번째 패스: %6ld µs  → TLB Hit\n",
           SMALL_PAGES, small_ns / 1000);
    printf("큰  세트 (%5d 페이지, 256MB) 2번째 패스: %6ld µs  → TLB Miss\n",
           LARGE_PAGES, large_ns / 1000);
    printf("차이: %.1f배\n", (double)large_ns / small_ns * ((double)SMALL_PAGES / LARGE_PAGES));
    printf("(접근 횟수 차이 보정: 페이지당 시간 기준)\n");

    free(small_arr);
    free(large_arr);
    return 0;
}
