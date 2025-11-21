# ENPM691 Homework Assignment 04: Memory Layout and Automated Debugging with Variable Declarations

**Vedant Bhalerao**  
vedantb@umd.edu

## Abstract

This assignment investigates how C programs organize variables of different storage classes in memory. Using the program address_layout.c, we examined the runtime addresses of local, global (initialized and uninitialized), static, and dynamically allocated variables. By combining program output with automated inspection using a GDB script and the pwndbg extension, we mapped each variable type to its corresponding memory segment: stack, heap, data, and BSS. The analysis also included disassembled code to illustrate how the compiler accesses variables based on their storage class. Results highlight the role of initialization, lifetime, and storage class in memory placement, providing hands-on understanding of program memory organization and debugging practices.

**Index Terms:** Memory segmentation, Stack memory, Heap memory, Data segment, BSS segment, Local variables, Global variables, Static variables, Dynamic memory allocation, GDB debugging, pwndbg, C programming

## I. Introduction

Understanding how a program's variables are arranged in memory is an important step in low-level programming and debugging. In C, variable addresses are determined by their storage class, initialization state, and lifetime. Program memory is divided into several regions: the stack stores automatic local variables, the heap holds dynamically allocated objects, the data segment contains initialized globals and statics, and the BSS segment is used for uninitialized globals and statics.

The goal of this assignment is to investigate these memory regions using the program address_layout.c, which declares variables across different storage classes and prints their addresses at runtime. To extend this analysis, we employ gdb together with pwndbg scripting tools to automate the process of collecting variable addresses, inspecting memory layouts, and viewing the assembly code that accesses these variables. Instead of manually entering debugger commands, a scripted workflow ensures consistency, reduces effort, and makes it easier to reproduce results across runs. This experiment connects theoretical knowledge of program memory organization with a practical, automated debugging approach, showing how compilers, linkers, and operating systems work together to manage program data.

## II. Methodology

To investigate how variables of different storage classes are mapped to memory in C, a structured compile–run–debug workflow was employed. All experiments were performed on a Kali Linux system using the GNU Debugger (gdb) enhanced with pwndbg to automate memory inspection and analyze program execution.

### System Information:

- Operating System: Kali Linux 6.12.38+kali-amd64
- Kernel Version: 6.12.38-1kali1
- Architecture: x86 (32-bit mode for this assignment)
- Compiler: GCC (GNU Compiler Collection)
- GCC Version: 14.3.0 (Debian 14.3.0-5)

### A. Overview

The program address_layout.c was designed to declare variables of several storage classes:

- Local (automatic) variables stored on the stack,
- Global initialized and uninitialized variables,
- Static local variables stored in the data segment, and
- Heap variables dynamically allocated with malloc.

At runtime, the program prints the addresses of these variables. By combining this output with automated debugger inspection, each variable was correlated with its memory segment (stack, heap, data, or BSS).

### B. Compilation

The program was compiled with debugging symbols, targeting 32-bit execution, and with optimization disabled to simplify memory and assembly analysis. Each flag served a specific purpose:

- `-m32`: Generate 32-bit object code to match IA-32 memory layout and stack frames.
- `-g`: Include debugging symbols, enabling source-level debugging in gdb.
- `-O0`: Disable compiler optimizations to preserve the original variable layout and prevent the compiler from reordering or removing variables.

The compilation command used was:
```
gcc -m32 -g -O0 address_layout.c -o address_layout
```

### C. Execution Verification

The compiled binary was run normally to ensure correct execution and to record the printed addresses of variables for later comparison with debugger output.

### D. Automated GDB Script

An automated GDB script (auto_script.gdb) was used to standardize memory inspection and reduce manual steps. The script performed the following tasks:

- Disabled pagination and enabled logging to capture all outputs in a text file.
- Set a breakpoint at main and ran the program to pause execution before variable initialization.
- Displayed the process memory map to correlate stack, heap, and data segments with variable addresses.
- Printed the addresses of local, static, global, and heap pointer variables using exact names from the source code.
- Disassembled the main function to observe how compiler-generated instructions access these variables.
- Attempted to dump memory contents at heap pointers to inspect allocated memory blocks.
- Listed all global and static symbols to confirm their placement in the data or BSS segments.
- Quit automatically after logging all relevant information.

### E. Interactive Debugging Procedure

Further exploration was performed interactively using gdb with pwndbg:

1. Launched GDB: `gdb ./address_layout`
2. Set a breakpoint at main: `break main`
3. Ran the program: `run`, pausing execution at the start of main.
4. Inspected variables and memory using pwndbg commands:
   - `info variables` to list global and static variable addresses,
   - `print &varname` to obtain stack variable addresses,
   - `info proc mappings` to observe memory segments,
   - `disassemble main` to examine instructions accessing variables.

This methodology automated data collection, correlated variable declarations with their runtime memory placement, and provided a reproducible workflow for analyzing stack, heap, data, and BSS layouts.

## III. Results

All experiments were performed with the same compilation flags (`gcc -m32 -g -O0`) and executed under gdb using the pwndbg extension. For each variable type, runtime addresses, representative disassembly, and debugger observations are summarized below.

### A. Local Variables (automatic stack storage)

- **Declarations:**
  ```c
  int local_var_1 = 0;
  int local_var_2 = 0;
  ```

- **Runtime Output:**
  ```
  Local var 1 address: 0xffc9d544
  Local var 2 address: 0xffc9d540
  ```

- **Disassembly (main, initialization):**
  ```
  mov DWORD PTR [ebp-0x14],0x0
  mov DWORD PTR [ebp-0x18],0x0
  ```
  
  Local variables are allocated on the stack at offsets -0x14 and -0x18 from EBP.

- **Debugger Snapshot:** Stack inspection around ESP confirmed reserved slots for these locals, located at high memory addresses near 0xffc9d54x.

### B. Heap Variables (dynamic allocation)

- **Declarations:**
  ```c
  int *ptr_1 = malloc(100);
  int *ptr_2 = malloc(100);
  ```

- **Runtime Output:**
  ```
  Heap var 1 address: 0x57f2c1a0
  Heap var 2 address: 0x57f2c210
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
  
  Each malloc returns a pointer in EAX, which is stored in a local stack slot.

- **Debugger Snapshot:** After the first allocation, EAX held 0x57f2c1a0, saved at [ebp-0xc]. The second allocation returned 0x57f2c210, saved at [ebp-0x10]. Memory dumps at these addresses confirmed allocated heap blocks.

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
  Global var 1 address: 0x565bd018
  Global var 2 address: 0x565bd01c
  Global (uninit) var 1 address: 0x565bd020
  Global (uninit) var 2 address: 0x565bd024
  ```

- **Symbol Table (from gdb info variables):**
  ```
  global_var_1, global_var_2 -> .data
  global_uninit_var_1, global_uninit_var_2 -> .bss
  ```
  
  Initialized globals reside in the .data segment, while uninitialized globals are allocated in .bss.

- **Debugger Snapshot:** Global variables are grouped in mid–low memory addresses (0x565bd0xx), consistent with static data storage.

### D. Static Local Variables

- **Declarations:**
  ```c
  static int static_var_1 = 0;
  static int static_var_2 = 0;
  ```

- **Runtime Output:**
  ```
  Static Local var 1 address: 0x565bd028
  Static Local var 2 address: 0x565bd02c
  ```

- **Symbol Table (from gdb info variables):**
  ```
  static_var_1, static_var_2 -> .data
  ```
  
  Static locals, though declared inside main, are stored alongside global variables in the .data segment because their lifetime spans the entire program execution.

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

The experiment demonstrates how the compiler and linker organize variables into distinct memory regions based on storage class, lifetime, and initialization. Rather than treating all variables uniformly, the program's memory is segmented into stack, heap, data, and BSS regions, with each optimized for access patterns and lifespan. The use of pwndbg allowed automation of memory inspection, correlating variable declarations with their actual runtime addresses and revealing compiler-generated assembly access patterns.

### A. Local Variables (automatic)

Local variables (`local_var_1`, `local_var_2`) were allocated on the stack. The disassembly confirms this:
```
mov DWORD PTR [ebp-0x14],0x0
mov DWORD PTR [ebp-0x18],0x0
```

The compiler reserved stack frame space and accessed these variables using offsets from EBP. The runtime addresses (0xffc9d540, 0xffc9d544) show that the stack resides at high virtual addresses and grows downward, consistent with IA-32 stack conventions.

### B. Heap Variables (dynamic allocation)

Heap variables (`ptr_1`, `ptr_2`) were allocated using malloc. Each allocation returned a pointer in the EAX register, which was stored in local stack slots. The addresses (0x57f2c1a0, 0x57f2c210) fall within the heap region, which grows upward at runtime. Memory dumps performed via the automated GDB script verified that these allocations correspond to distinct heap blocks, illustrating dynamic memory management.

### C. Global Variables

Initialized globals (`global_var_1`, `global_var_2`) were stored in the .data segment, while uninitialized globals (`global_uninit_var_1`, `global_uninit_var_2`) were placed in .bss. The addresses (0x565bd018–0x565bd024) cluster together in mid–low memory, confirming their placement in contiguous static regions. This organization enables initialized data to be embedded in the binary, while reserving space for uninitialized data without storing values.

### D. Static Local Variables

Static locals (`static_var_1`, `static_var_2`), though declared inside main, were stored alongside globals at addresses 0x565bd028 and 0x565bd02c. This confirms that static variables retain their values across function calls, and the compiler treats them as having global lifetime, placing them in the data segment.

### E. Comparison and Observations

- Local (automatic) variables reside on the stack and are accessed via EBP offsets, with lifetime limited to the function scope.
- Heap variables are dynamically allocated at runtime; their addresses are managed by the heap allocator and persist until explicitly freed.
- Initialized globals are stored in .data, while uninitialized globals occupy .bss.
- Static local variables, despite lexical scoping, reside in the same memory region as globals to preserve their values throughout program execution.

This experiment confirms that C storage classes map directly to different memory segments. By automating inspection with pwndbg, we were able to correlate source code declarations, runtime addresses, and assembly instructions, deepening understanding of the compiler's memory management and runtime behavior.

## V. Appendix

### Appendix A: Source file

See `src/address_layout.c`

### Appendix B: Makefile

See `Makefile` in this directory.

### Appendix C: Automated GDB Script

See `src/auto_script.gdb` for the complete automated debugging script. The script:
- Sets up logging to `address_layout_32_simple_output.txt`
- Sets breakpoints and runs the program
- Displays memory mappings and variable addresses
- Disassembles main function
- Dumps heap memory contents
- Lists all symbols and variables

### Appendix D: Running the Automated Script

To run the automated GDB script:

```bash
make auto-debug
```

Or manually:

```bash
gdb -x src/auto_script.gdb ./address_layout
```

The script will generate `address_layout_32_simple_output.txt` with all debugger output.

## References

[1] R. Bryant and D. O'Hallaron, Computer Systems: A Programmer's Perspective, 3rd ed. Boston, MA: Pearson, 2016.

[2] A. S. Tanenbaum and H. Bos, Modern Operating Systems, 4th ed. Pearson, 2015.

[3] Free Software Foundation, GDB: The GNU Project Debugger, [Online]. Available: https://www.gnu.org/software/gdb/

[4] Pwndbg Team, Pwndbg: GDB plugin for exploit development, [Online]. Available: https://github.com/pwndbg/pwndbg

