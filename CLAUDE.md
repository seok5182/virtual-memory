# 프로젝트 컨텍스트: 가상 메모리 학습 프로젝트

## 프로젝트 목적
운영체제의 가상 메모리 시스템을 C 코드로 직접 실험하고 분석하는 학습 프로젝트.
이론이 아닌 실증 데이터로 개념을 확인하는 것이 목표.

## 기술 환경
- **OS**: macOS (Darwin, ARM64 / Apple Silicon)
- **분석 도구**: `vmmap [PID]`
- **언어**: C (GCC)
- **페이지 크기**: 16KB (macOS ARM64 기준)

## 완료된 실험

### 실험 1: ASLR 확인 (2026-03-15) - `memory_test.c`
- 전역 변수(Data), 지역 변수(Stack) 가상 주소 출력
- 두 프로세스 실행 시 주소가 다름 → ASLR 확인

### 실험 2: Demand Paging 확인 (2026-03-16) - `heap_test.c`
- `malloc(10MB)` 후 RESIDENT 0K → `memset()` 후 RESIDENT 10MB
- 지연 할당(Demand Paging) 실증

### 실험 3: Stack Overflow (2026-03-17) - `stack_overflow_test.c`
- 스택 프레임 1개 = 1,088 bytes (`buffer[1024]` + 오버헤드)
- depth 7,689번째 호출에서 Segfault
- Guard Page(`0x16a7f0000`) 침범으로 종료
- Stack Overflow는 가상 메모리에서 터짐 (RAM 무관)

### 실험 4: mmap() (2026-03-18) - `mmap_test.c`
- `mmap()` 직후 RESIDENT 0K → 파일 전체 읽기 후 RESIDENT 10.0M (Demand Paging 확인)
- 힙과 달리 DIRTY 0K → 디스크 원본과 RAM 내용 일치 (Clean)
- vmmap에서 `mapped file`로 표시되는 별도 영역 확인

### 실험 5: fork() + CoW (2026-03-18) - `fork_cow_test.c`
- fork() 직후 자식 MALLOC_LARGE RESIDENT 16K → 물리 RAM 복사 안 함 (CoW 공유)
- 자식 memset('B') 후 RESIDENT 10MB → CoW 발동으로 페이지 복사
- 부모 data[0] = 'A' 유지 → 프로세스 격리 확인

### 실험 6: TLB Miss (2026-03-19) - `tlb_test.c`
- 작은 세트(512페이지, 8MB) vs 큰 세트(16,384페이지, 256MB) 2번째 패스 시간 비교
- Apple Silicon L2 캐시(16MB)가 TLB Miss 페널티를 흡수 → 차이 미미
- TLB/L1/L2 캐시 계층 학습

## 진행 예정 실험
- 추가 실험 미정
- [ ] 실험 6: TLB miss - 순차 vs 랜덤 접근 성능 측정

## 학습자 배경
- 전자공학 전공, 하드웨어(CPU/RAM) 기초 지식 보유
- C 언어 익숙하지 않음 (포인터 개념 학습 중)
- Docker 경험 있음 (비유로 활용 가능)
- 개념 설명 시 하드웨어-소프트웨어 연결 고리 포함할 것
