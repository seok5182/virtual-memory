# AI Context: Virtual Memory Analysis Project

## System Environment
- **OS**: Darwin (macOS)
- **Primary Tool**: `vmmap` (for memory layout analysis)

## Core Components
- `memory_test.c`: Prints virtual addresses of global/local variables and PID.
- `memory_test`: Compiled binary (Mach-O).

## Experimental Data (Captured 2026-03-15)
- **Process A (PID 17045)**:
  - Global (Data): `0x102510000`
  - Local (Stack): `0x16d8f6e28`
- **Process B (PID 17058)**:
  - Global (Data): `0x100104000`
  - Local (Stack): `0x16fd02e28`
## Key Analytical Insights
1. **ASLR Active**: Addresses change per execution, confirming macOS Address Space Layout Randomization.
2. **Identical Virtual Addresses (Theoretically)**: 
   - Without ASLR, **both Global and Local variables** would have identical virtual addresses across different processes.
   - Global: Fixed by the Linker in the binary.
   - Local: Fixed by the OS assigning the same initial Stack Pointer (SP).
3. **Process Isolation (Docker Analogy)**: 
...
   - Unlike Docker's `host` mode (sharing), Virtual Memory is more like **Docker's Bridge mode**.
   - Each process has a private "IP" (Virtual Address), mapped to "Host IP" (Physical RAM) by the MMU.
3. **Memory Topology**:
   - Data segment typically resides in the `0x10...` range.
   - Stack segment resides in the high `0x16...` range (Darwin arm64 convention).
4. **Demand Paging**: The OS doesn't map RAM immediately; it maps "on-demand" when the address is first accessed (Lazy Allocation).

## Technical Context for Gemini
- Refer to these PID results for comparison if the user asks about previous experiments.
- Use `vmmap [PID]` to assist in deeper memory mapping inquiries.
- Focus on Stack, Data, and Heap segment distinctions.
