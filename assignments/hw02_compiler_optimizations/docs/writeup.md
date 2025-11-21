# ENPM691 Homework Assignment 02: Compiler Optimizations and Debugging through Multiplication Functions

**Vedant Bhalerao**  
vedantb@umd.edu

## Abstract

This lab investigates how modern C compilers optimize multiplication by constant operands through strength reduction technique. Instead of doing general-purpose multiply instructions, compilers generate equivalent sequences of adds, subtracts, and shifts that are computationally preferable. Three programs were implemented to multiply an integer by 3, 7, and 12, respectively, and their compiled binaries were examined using gdb with pwndbg. The experiments demonstrate obvious optimization methods: shift–subtract pairs for small constants, combined shift–add sequences for larger constants, and add operations for constants near powers of two. With the use of disassembly and debugger analysis, this work shows how compiler optimizations affect low-level code generation and execution behavior and they shed light on instruction-level efficiency and design trade-offs.

**Index Terms:** Compiler optimization, strength reduction, multiplication by constants, assembly analysis, GDB debugging, C programming, instruction-level efficiency

## I. Introduction

When a C program is compiled, high-level instructions such as multiplication are translated into low-level assembly instructions that the processor can execute. While multiplication can be done directly using a multiply instruction, compilers typically optimize this type of operation by replacing it with a sequence of shifts, additions, or subtractions if the multiplier is a constant. This operation is known as strength reduction, which allows the compiler to generate faster and more efficient code since shifts and additions are computationally less expensive than multiplication on most architectures.

The objective of this assignment is to code simple C programs that multiply a variable by constant integers, compile them, and then examine the generated assembly code using a debugger. By comparing the assembly outputs for different multipliers, we can observe how the compiler selects different optimization methods, such as the replacement of multiplications with shift-and-add sequences. This provides hands-on insight into the ways that compilers optimize at the instruction level and helps to understand the trade-offs of low-level optimization.

## II. Methodology

To analyze how the compiler implements multiplication by constant values, a fixed workflow was used. All experiments were performed on a Kali Linux system and followed a consistent compile-run-debug procedure to ensure that we can compare each of the results.

### System Information:

- Operating System: Kali Linux 6.12.38+kali-amd64
- Kernel Version: 6.12.38-1kali1
- Architecture: x86_64 (64-bit)
- Compiler: GCC (GNU Compiler Collection)
- GCC Version: 14.3.0 (Debian 14.3.0-5)

### A. Overview

There were 3 programs that were defined which included a function performing an integer multiplication by a constant (for example, mulBy3, mulBy7, mulBy12). The binaries were compiled and then examined both in disassembly and dynamically. The same compilation flags and debugger settings were used for all experiments to produce comparable assembly output.

### B. Compilation

Binaries were produced using gcc with options selected to expose readable and reproducible assembly and debugger behavior. The key flags used were selected based on GCC documentation.

- `-m32`: produce 32-bit object code on an x86_64 host so the 32-bit calling convention and register usage are observed.
- `-g`: include debug symbols so gdb can map instructions to source lines and show variable/argument names.
- `-fno-stack-protector`: disable stack canaries to avoid extra instrumentation instructions in the binary.
- `-no-pie` and `-fno-pic`: disable position-independent code and executables so addresses in the disassembly remain fixed.
- `-z execstack`: mark the stack as executable (included to match the original build configuration for this assignment).

These flags were used consistently for every source file to ensure the assembly patterns produced by the compiler are comparable.

### C. Execution check

Each compiled binary was executed normally (e.g., `./mulby3`) prior to debugging to check correct runtime behavior and output.

### D. Debugger workflow

The dynamic inspection used gdb with the pwndbg extension (pwndbg provides enhanced, formatted panes for registers, disassembly and stack). The standard debugger workflow for every test case was:

1. Launch the debugger with the target binary: `gdb <binary>`.
2. Set a breakpoint at the function under investigation (for example, `break mulBy3`).
3. Run the program within the debugger using `run`. Execution halts when the breakpoint is reached.
4. Inspect the following views to correlate source and machine-level behavior:
   - **Registers**: view CPU registers (EAX, EBX, ECX, EDX, EIP, EBP, ESP). Registers show argument values and the return-value register (EAX) used by the calling convention.
   - **Disassembly**: inspect the function's assembly instructions using the disassembly pane or `disassemble <function>`. This reveals the exact instruction sequence the compiler emitted for the multiplication.

### E. Controlled variations

To isolate how multipliers influence code generation, the same compile and debugger procedure was repeated for multiple constant multipliers (for example, 3, 7, 12). Only the constant in the source was changed between runs; compilation flags, debugger breakpoints and inspection steps remained identical. This control ensures observed differences in assembly are attributable to compiler optimizations for the constant multiplier rather than environmental changes.

## III. Results

All test programs were compiled with the same flags and executed under gdb with pwndbg. For each multiplier tested, the runtime output, the disassembly of the function, and key register/state snapshots observed at the breakpoint are reported below.

### A. mulBy3 (multiplier = 3)

- **Function:**
  ```c
  int mulBy3(int x) { return x * 3; }
  ```

- **Runtime Output:**
  ```
  Result = 15
  ```

- **Disassembly (function body):**
  ```
  push ebp
  mov ebp,esp
  mov edx,DWORD PTR [ebp+0x8]
  mov eax,edx
  add eax,eax
  add eax,edx
  pop ebp
  ret
  ```
  
  Here, the compiler optimized the multiplication by 3 into a sequence of additions: first doubling EAX (`add eax,eax`) and then adding the original value (`add eax,edx`). This avoids using a costly `imul` instruction.

- **Register/step snapshots observed at breakpoint (representative):**
  - After `mov edx,[ebp+8]`: EDX = 5
  - After `mov eax,edx`: EAX = 5
  - After `add eax,eax`: EAX = 10
  - After `add eax,edx`: EAX = 15
  
  This step-by-step evolution clearly demonstrates how the input (x = 5) is transformed into the result (15).

### B. mulBy7 (multiplier = 7)

- **Function:**
  ```c
  int mulBy7(int x) { return x * 7; }
  ```

- **Runtime Output:**
  ```
  Result = 35
  ```

- **Disassembly (function body):**
  ```
  push ebp
  mov ebp,esp
  mov edx,DWORD PTR [ebp+0x8]
  mov eax,edx
  shl eax,0x3
  sub eax,edx
  pop ebp
  ret
  ```
  
  Here, the multiplication by 7 is optimized into a left shift by 3 (`shl eax,0x3`, equivalent to multiplying by 8) followed by a subtraction of the original value (`sub eax,edx`). This is a faster sequence than using a general `imul` instruction.

- **Register/step snapshots observed at breakpoint (representative):**
  - After `mov edx,[ebp+8]`: EDX = 5
  - After `mov eax,edx`: EAX = 5
  - After `shl eax,0x3`: EAX = 40
  - After `sub eax,edx`: EAX = 35
  
  These snapshots confirm the shift–subtract optimization that produces the correct result of 35 for input 5.

### C. mulBy12 (multiplier = 12)

- **Function:**
  ```c
  int mulBy12(int x) { return x * 12; }
  ```

- **Runtime Output:**
  ```
  Result = 60
  ```

- **Disassembly (function body):**
  ```
  push ebp
  mov ebp,esp
  mov edx,DWORD PTR [ebp+0x8]
  mov eax,edx
  add eax,eax
  add eax,edx
  shl eax,0x2
  pop ebp
  ret
  ```
  
  The compiler breaks the multiplication into intermediate steps. First, it computes (2x + x) = 3x. Then it applies a left shift by 2 (≪ 2), which multiplies the result by 4, yielding 12x. This avoids using a costly general-purpose multiplication instruction.

- **Register/step snapshots observed at breakpoint (representative):**
  - After `mov edx,[ebp+8]`: EDX = 5
  - After `mov eax,edx`: EAX = 5
  - After two adds (`add eax,eax` then `add eax,edx`): EAX = 15
  - After `shl eax,0x2`: EAX = 60
  
  These values verify the compiler's shift–add strategy for generating the final result.

### D. Summary table of observed transformations

| Multiplier | Observed assembly pattern |
|------------|---------------------------|
| 3 | `eax=edx; eax+=eax; eax+=edx;` |
| 7 | `eax=edx; shl eax,3; sub eax,edx;` |
| 12 | `eax=edx; eax+=eax; eax+=edx; shl eax,2;` |

## IV. Discussion

The results clearly show that the compiler does not use the same strategy for every multiplication by a constant. Instead, it chooses instruction sequences that minimize execution cost based on the binary representation of the multiplier. This optimization is known as strength reduction, where multiplications are replaced with cheaper operations such as shifts, additions, and subtractions.

### A. mulBy3

The multiplication by 3 was implemented as:
```
eax = edx
add eax, eax    ; eax = 2 * x
add eax, edx    ; eax = 2 * x + x = 3 * x
```

Here the compiler replaced the multiply by 3 with a simple doubling followed by an addition. Since both additions are single-cycle operations, this is faster than using the `imul` instruction.

### B. mulBy7

The multiplication by 7 was implemented as:
```
eax = edx
shl eax, 0x3    ; eax = 8 * x
sub eax, edx    ; eax = 8 * x - x = 7 * x
```

Here the compiler took advantage of the fact that 7 = 8 − 1. A left shift by 3 bits computes 8×x, and subtracting x yields 7 × x. This is again more efficient than a general multiply.

### C. mulBy12

The multiplication by 12 was implemented as:
```
eax = edx
add eax, eax    ; eax = 2 * x
add eax, edx    ; eax = 3 * x
shl eax, 0x2    ; eax = 3 * x * 4 = 12 * x
```

The compiler recognized that 12 = 3 × 4. It first computed 3 × x using the same method as in mulBy3, then performed a left shift by two bits (equivalent to multiplying by 4). This reduces the multiplication to a combination of additions and a single shift.

### D. Comparison

- For small constants such as 3, the compiler uses additions.
- For values like 7 that are close to powers of two, the compiler uses a shift to compute the nearest power of two and then subtracts the original value.
- For larger constants like 12, the compiler decomposes the value into smaller factors, combining additions with shifts.

Overall, the differences demonstrate how the compiler tailors the generated code depending on the structure of the multiplier in binary. Multiplications by powers of two translate to pure shifts, while other constants are expressed as combinations of shifts, adds, and subtracts. This confirms that the compiler consistently applies arithmetic transformations to avoid expensive multiply instructions, especially under optimization.

## V. Appendix

### Appendix A: Source files

See `src/` directory for:
- `mulby3.c`
- `mulby7.c`
- `mulby12.c`

### Appendix B: Makefile

See `Makefile` in this directory.

### Appendix C: GDB Command Files

These commands can be used with gdb to reproduce the disassembly / breakpoint traces:

**For mulby3:**
```
gdb ./mulby3
break mulBy3
run
disassemble mulBy3
quit
```

**For mulby7:**
```
gdb ./mulby7
break mulBy7
run
disassemble mulBy7
quit
```

**For mulby12:**
```
gdb ./mulby12
break mulBy12
run
disassemble mulBy12
quit
```

## References

[1] W. Stallings, Computer Security: Principles and Practice, 3rd ed. Pearson, 2017.

[2] D. A. Patterson and J. L. Hennessy, Computer Organization and Design: The Hardware/Software Interface, 5th ed. Morgan Kaufmann, 2017.

[3] GNU Project, "Using the GNU Compiler Collection (GCC) Documentation." [Online]. Available: https://gcc.gnu.org/onlinedocs/. [Accessed: Sep. 22, 2025].

[4] GNU Project, "GDB: The GNU Project Debugger." [Online]. Available: https://sourceware.org/gdb/current/onlinedocs/. [Accessed: Sep. 22, 2025].

[5] pwndbg Developers, "pwndbg – A GDB plugin for exploit development." [Online]. Available: https://github.com/pwndbg/pwndbg. [Accessed: Sep. 22, 2025].

[6] S. S. Muchnick, Advanced Compiler Design and Implementation. Morgan Kaufmann, 1997.

