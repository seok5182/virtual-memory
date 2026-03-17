#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open()
#include <unistd.h> // getpid(), close()
#include <sys/mman.h> // mmap(), munmap()
#include <sys/stat.h> // fstat()

int main() {
    int pid = getpid();
    printf("PID : %d\n", pid);

    // 1. 파일 열기
    int fd = open("testfile.bin", O_RDONLY);
    if (fd == -1) {
        printf("파일 열기 실패! testfile.bin을 먼저 생성하세요.\n");
        return 1;
    }

    // 2. 파일 크기 확인
    struct stat st;
    fstat(fd, &st);
    size_t size = st.st_size;
    printf("파일 크기 : %zu bytes\n", size);

    // 3. mmap으로 가상 주소에 매핑
    char *ptr = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("mmap 실패!\n");
        return 1;
    }
    
    printf("매핑된 가상 주소: %p\n", (void *)ptr);
    printf("1단계: 매핑만 한 상태. 엔터를 치면 파일을 읽습니다.\n");
    printf("(지금 'vmmap %d' 확인하세요!)\n", pid);
    getchar();

    // 4. 실제 파일 내용 접근
    long sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += ptr[i];  // 전체 페이지 접근 → Page Fault 발생
    }
    printf("sum: %ld\n", sum);

    printf("2단계: 파일 전체 읽기 완료. 엔터를 치면 종료합니다.\n");
    printf("(다시 'vmmap %d' 확인하세요!)\n", pid);
    getchar();

    // 5. 해제
    munmap(ptr, size);
    close(fd);

    return 0;
}