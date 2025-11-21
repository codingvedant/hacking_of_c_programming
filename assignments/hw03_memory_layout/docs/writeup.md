# ENPM691 Homework Assignment 03: Memory Layout and Debugging through Variable Declarations

**Vedant Bhalerao**  
vedantb@umd.edu

## Abstract

This experiment explores memory segmentation in C programs by analyzing the storage of variables from different storage classes. We developed a program (address_layout.c) to print the memory addresses of local, global, static, and dynamically allocated variables. The results demonstrate their placement in distinct memory segments: the stack, data, BSS, and heap. By correlating these observations with assembly code and debugger output, we verify how the compiler manages initialized versus uninitialized data. The project strengthens practical knowledge of memory organization, runtime behavior, and debugging techniques.

**Index Terms:** Memory segmentation, Stack memory, Heap memory, BSS segment, Data segment, Static variables, Global variables, Local variables, Dynamic memory allocation, GDB debugging, pwndbg, C programming

## I. Introduction

Understanding how a program's variables are arranged in memory is a key step in learning low-level programming and debugging. When a C program is compiled, variables are not stored arbitrarily; instead, their location depends on their storage class, initialization, and lifetime. Broadly, program memory is divided into the stack, heap, data, and BSS segments, with each region handling different types of variables. The stack is used for automatic local variables, the heap manages dynamically allocated memory, the data segment stores initialized globals and statics, and the BSS segment contains uninitialized globals and statics.

The aim of this assignment is to explore these concepts by analyzing a simple C program, address_layout.c, which declares variables of various storage classes: local, global, static, and dynamically allocated, and prints their addresses at runtime. Using both program output and debugger inspection in gdb with pwndbg, we can observe the relative positions of these variables in memory and examine the assembly code that accesses them. This experiment bridges the gap between theoretical memory organization and practical implementation, reinforcing how compilers, linkers, and the operating system collaborate to manage program data in the process address space.

## II. Methodology

To investigate how different storage classes in C are mapped to memory regions, a systematic compile–run–debug workflow was followed. All experiments were carried out on a Kali Linux machine, using the GNU debugger (gdb) with the pwndbg extension to analyze program execution and memory addresses.

### System Information:

- Operating System: Kali Linux 6.12.38+kali-amd64
- Kernel Version: 6.12.38-1kali1
- Architecture: x86_64 (64-bit)
- Compiler: GCC (GNU Compiler Collection)
- GCC Version: 14.3.0 (Debian 14.3.0-5)

### A. Overview

The program address_layout.c was designed to declare and initialize variables of different storage classes, including:

- Local (automatic) variables on the stack,
- Global initialized and uninitialized variables,
- Static local variables, and
- Heap variables allocated dynamically using malloc.

At runtime, the program prints the memory addresses of these variables. This output was combined with debugger inspection to identify the corresponding memory segments—stack, heap, data, and BSS.

### B. Compilation

The program was compiled using GCC with debugging symbols enabled and configured for 32-bit execution:

- `-m32`: generated 32-bit object code to analyze IA-32 stack frame and memory layout.
- `-g`: included debugging symbols to support source-level inspection in gdb.

The final binary was produced using:
```
gcc -m32 -g address_layout.c -o address_layout
```

### C. Execution Check

The binary was executed outside the debugger to confirm correct functionality and ensure all variables were properly allocated and initialized. The program's standard output displayed the addresses of local, global, static, and heap variables, which were saved for later analysis.

### D. Debugger Workflow

The binary was then analyzed using gdb with the pwndbg extension:

1. Launched the debugger: `gdb ./address_layout`
2. Set a breakpoint at main using `break main` to pause before variable initialization.
3. Ran the program with `run`, halting at the breakpoint.
4. Inspected program state using pwndbg commands:
   - `info variables` — listed global and static symbols with addresses.
   - `info registers` and `x/20x $esp` — displayed stack pointer and stack contents.
5. Observed heap allocations by examining return values of malloc, stored in registers (EAX) and stack slots.

This workflow ensured reproducible and verifiable results, correlating source-level variable declarations with their actual memory placement as determined by the compiler, linker, and runtime environment.

## III. Results

All experiments were conducted using the same compilation flags (`gcc -m32 -g`) and executed under gdb with the pwndbg extension. For each variable type, the runtime output, representative disassembly, and debugger snapshots at breakpoints are reported below.

### A. Local Variables (automatic stack storage)

- **Declarations:**
  ```c
  int local_var_1 = 0;
  int local_var_2 = 0;
  ```

- **Runtime Output:**
  ```
  Local var 1 address: 0xffcca554
  Local var 2 address: 0xffcca550
  ```

- **Disassembly (main, initialization):**
  ```
  mov DWORD PTR [ebp-0x14],0x0
  mov DWORD PTR [ebp-0x18],0x0
  ```
  
  Both locals are allocated on the stack relative to EBP, at offsets -0x14 and -0x18.

- **Debugger Snapshot:** Stack dump around ESP confirmed the reserved slots for local variables, located at high memory addresses near 0xffcca5xx.

### B. Heap Variables (dynamic allocation)

- **Declarations:**
  ```c
  int *ptr_1 = malloc(100);
  int *ptr_2 = malloc(100);
  ```

- **Runtime Output:**
  ```
  Heap var 1 address: 0x568301a0
  Heap var 2 address: 0x56830210
  ```

- **Disassembly (malloc usage):**
  ```
  push 0x64
  call malloc@plt
  mov DWORD PTR [ebp-0xc],eax
  ...
  push 0x64
  call malloc@plt
  mov DWORD PTR [ebp-0x10],eax
  ```
  
  Each call to malloc returned a heap address in EAX, which the compiler stored in local stack slots.

- **Debugger Snapshot:** After the first malloc, EAX contained 0x568301a0, which was saved at [ebp-0xc]. The second allocation returned 0x56830210, saved at [ebp-0x10].

### C. Global Variables (initialized vs uninitialized)

- **Declarations:**
  ```c
  int global_var_1 = 0;
  int global_var_2 = 0;
  int global_uninit_var_1;
  int global_uninit_var_2;
  ```

- **Runtime Output:**
  ```
  Global (uninit) var 1 address: 0x565ce020
  Global (uninit) var 2 address: 0x565ce024
  Global var 1 address: 0x565ce018
  Global var 2 address: 0x565ce01c
  ```

- **Symbol Table (from gdb info variables):**
  ```
  global_var_1, global_var_2 -> .data
  global_uninit_var_1, global_uninit_var_2 -> .bss
  ```
  
  Initialized globals are placed in the .data segment, while uninitialized ones are reserved in .bss.

- **Debugger Snapshot:** All global addresses were grouped in the mid–low address range (0x565ce0xx), consistent with static data storage.

### D. Static Local Variables

- **Declarations:**
  ```c
  static int static_var_1 = 0;
  static int static_var_2 = 0;
  ```

- **Runtime Output:**
  ```
  Static Local var 1 address: 0x565ce028
  Static Local var 2 address: 0x565ce02c
  ```

- **Symbol Table (from gdb info variables):**
  ```
  static_var_1, static_var_2 -> .data
  ```
  
  Despite being declared inside main, static locals are stored with globals in the data segment because their lifetime extends across the entire program run.

- **Debugger Snapshot:** The static variables' addresses appear immediately after the uninitialized globals, confirming compiler placement in the static data region.

### E. Summary Table of Observed Memory Regions

| Variable Type | Example | Region / Segment |
|---------------|---------|------------------|
| Local (auto) | local_var_1, local_var_2 | Stack (EBP offsets, high addresses) |
| Heap (malloc) | ptr_1, ptr_2 | Heap (dynamic, upward growth) |
| Global initialized | global_var_1, global_var_2 | .data segment |
| Global uninitialized | global_uninit_var_1, global_uninit_var_2 | .bss segment |
| Static local | static_var_1, static_var_2 | .data segment |

## IV. Discussion

The results clearly show that the compiler and linker place variables into different memory regions based on their storage class and initialization. Rather than treating all variables the same, the program's layout in virtual memory is segmented into stack, heap, data, and BSS, each optimized for variable lifetime and accessibility. This experiment highlights how the compiler generates distinct access patterns in assembly to match these regions.

### A. Local Variables (automatic)

Local variables such as `local_var_1` and `local_var_2` were stored on the stack. The disassembly showed:
```
mov DWORD PTR [ebp-0x14],0x0
mov DWORD PTR [ebp-0x18],0x0
```

Here, the compiler allocated space in the current stack frame and accessed the variables using offsets from EBP. Their printed addresses (0xffcca554, 0xffcca550) confirm that stack memory resides at high virtual addresses, growing downward.

### B. Heap Variables (dynamic)

Heap variables, allocated via malloc, resided in dynamically managed memory. Each call to malloc returned an address in the EAX register, which was then saved to the stack at slots reserved for ptr_1 and ptr_2. The runtime addresses (0x568301a0, 0x56830210) appeared in the heap region, which grows upward at runtime. This behavior reflects the dynamic nature of heap allocation.

### C. Global Variables

Initialized global variables (`global_var_1`, `global_var_2`) were placed in the .data segment, while uninitialized globals (`global_uninit_var_1`, `global_uninit_var_2`) were placed in the .bss segment. Their addresses (0x565ce018–0x565ce024) clustered together, confirming their storage in contiguous regions of static memory. This design allows the executable to store initial values in the binary for .data and simply reserve space for .bss.

### D. Static Local Variables

Although declared inside main, the static locals (`static_var_1`, `static_var_2`) were stored in the same area as globals, at addresses 0x565ce028 and 0x565ce02c. This confirms that static variables have program-wide lifetime, and the compiler treats them as global-scope variables placed in the data segment.

### E. Comparison

- Local (automatic) variables are stack-allocated with short lifetime, accessed via EBP offsets.
- Heap variables are dynamically allocated at runtime, with addresses managed by the heap allocator.
- Global initialized variables are stored in .data, while uninitialized globals go to .bss.
- Static locals, despite their lexical scope, share the global memory regions to preserve their values across function calls.

Overall, the differences demonstrate how C storage classes map directly to distinct memory regions in the process address space. This confirms that the compiler organizes variables not only by type but also by lifetime and initialization state, aligning with the theoretical model of stack, heap, .data, and .bss segments taught in systems programming.

## V. Appendix

### Appendix A: Source file

See `src/address_layout.c`

### Appendix B: Makefile

See `Makefile` in this directory.

### Appendix C: GDB Command Files

These command sequences can be passed to gdb (with pwndbg enabled) to reproduce the disassembly, memory inspections, and register traces used in the report:

```
gdb ./address_layout
break main
run
info registers
info variables
disassemble main
quit
```

## References

[1] R. Stallman, R. Pesch, S. Shebs, et al., "Debugging with GDB: The GNU Source-Level Debugger," Free Software Foundation, 12th ed., 2023. [Online]. Available: https://www.gnu.org/software/gdb/

[2] pwndbg contributors, "pwndbg: Exploit Development and Reverse Engineering with GDB," GitHub repository, 2024. [Online]. Available: https://github.com/pwndbg/pwndbg

[3] GNU Project, "GCC, the GNU Compiler Collection," Free Software Foundation, 2024. [Online]. Available: https://gcc.gnu.org/

[4] R. Bryant and D. O'Hallaron, "Computer Systems: A Programmer's Perspective," 3rd ed., Pearson, 2016.

[5] A. S. Tanenbaum and T. Austin, "Structured Computer Organization," 6th ed., Pearson, 2012.

