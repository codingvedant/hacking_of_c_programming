# Hacking-of-C — ENPM691 assignments (Vedant Bhalerao)

This repository contains my ENPM691 assignments on low-level memory/ binary exploitation and C programming — lab writeups, source code, and build scripts.

> ⚠️ **Important:** This repository contains exploit code, proof-of-concept binaries, and debugging scripts intended **only** for educational use inside a closed lab environment. Do **not** run these programs on production machines. See `SAFE_USAGE.md`.

## Structure

- `assignments/` — per-homework folders with `src/`, `docs/`, `Makefile`.
- `makefiles/` — shared build scripts.
- `misc/` — gdb scripts, helper tools (if needed).
- `docs/` — original PDF writeups (kept for completeness).

## How to build an assignment (example)

```bash
cd assignments/hw01_memory_alignment
make              # builds hw1_sequential, hw1_reordered, hw1_packed
make run          # builds and runs all programs
make debug        # builds and runs with GDB debugger
make clean        # removes compiled binaries
```

## Assignments

### Homework 01: Memory Allocation and Alignment in C

Investigates how the C compiler handles memory alignment and padding for different variable types. Demonstrates three approaches: sequential declaration, reordered variables, and packed structures.

**Location:** `assignments/hw01_memory_alignment/`

### Homework 02: Compiler Optimizations and Debugging through Multiplication Functions

Investigates how modern C compilers optimize multiplication by constant operands through strength reduction technique. Examines how compilers generate equivalent sequences of adds, subtracts, and shifts instead of general-purpose multiply instructions. Three programs multiply integers by 3, 7, and 12, with assembly analysis using GDB.

**Location:** `assignments/hw02_compiler_optimizations/`

**Note:** Requires 32-bit compilation support (`gcc-multilib` package).

### Homework 03: Memory Layout and Debugging through Variable Declarations

Explores memory segmentation in C programs by analyzing the storage of variables from different storage classes. The program prints memory addresses of local, global, static, and dynamically allocated variables to demonstrate their placement in distinct memory segments: the stack, data, BSS, and heap. Uses GDB with pwndbg to correlate observations with assembly code.

**Location:** `assignments/hw03_memory_layout/`

**Note:** Requires 32-bit compilation support (`gcc-multilib` package).

### Homework 04: Memory Layout and Automated Debugging with Variable Declarations

Investigates how C programs organize variables of different storage classes in memory using automated GDB scripts. Extends hw03 with automated debugging workflow using a GDB script to standardize memory inspection, collect variable addresses, inspect memory layouts, and view assembly code. Emphasizes reproducible debugging practices and automated analysis.

**Location:** `assignments/hw04_automated_debugging/`

**Note:** Requires 32-bit compilation support (`gcc-multilib` package). Includes automated GDB script for memory analysis.

### Homework 05: Executing Shellcode via Return-Address Overwrite and Function-Pointer Call

Explores low-level program control and stack memory manipulation by implementing two shellcode-based examples. Demonstrates two methods to transfer execution to shellcode: overwriting the return address (shell1) and using a function pointer (shell2). Both programs embed 32-bit assembly to invoke exit(2), providing a safe proof-of-concept. Uses GDB with pwndbg to observe runtime behavior, memory layout, and instruction execution.

**Location:** `assignments/hw05_shellcode_execution/`

**Note:** Requires 32-bit compilation support (`gcc-multilib` package). ⚠️ **Security Warning:** This assignment demonstrates exploitation techniques for educational purposes only. Programs require executable stack (`-z execstack`) and disabled security mitigations.

### Homework 06: Stack-Based Privilege Escalation via JMP %ESP Buffer Overflow Exploitation

Explores stack-based buffer overflow exploitation and privilege escalation in Linux by implementing a SUID shell acquisition through the JMP %ESP technique. The program contains a deliberate buffer overflow vulnerability that is exploited to execute injected shellcode with elevated privileges. Uses GDB with pwndbg to analyze memory layout, identify the JMP %ESP gadget, and verify successful code execution. Demonstrates how stack memory corruption, return address overwriting, and shellcode injection interact to achieve privilege escalation.

**Location:** `assignments/hw06_suid_privilege_escalation/`

**Note:** Requires 32-bit compilation support (`gcc-multilib` package). ⚠️ **CRITICAL WARNING:** This assignment demonstrates privilege escalation techniques. **MUST** be run in an isolated VM environment. Requires SUID root privileges for demonstration. See assignment README for setup instructions.

### Homework 07: Return Oriented Programming (ROP) Exploitation - Bypassing Non-Executable Stack Protection

Details the exploitation of a buffer overflow vulnerability using Return Oriented Programming (ROP) techniques to bypass Non-Executable (NX) stack protection. Demonstrates how to chain together existing code segments (gadgets) within a vulnerable binary to achieve arbitrary code execution and spawn a shell, despite the presence of modern security mitigations. Uses GDB, pwntools, and ropper for gadget discovery, payload construction, and practical exploitation. Shows how code reuse attacks bypass NX protection by leveraging the program's own executable code segments.

**Location:** `assignments/hw07_rop_exploitation/`

**Note:** Requires 32-bit compilation support (`gcc-multilib` package), Python 3 with pwntools, and ropper. ⚠️ **Security Warning:** This assignment demonstrates advanced ROP exploitation techniques. **MUST** be run in an isolated VM environment. Requires ASLR to be disabled for demonstration. See assignment README for setup instructions.

### Homework 08: Global Offset Table (GOT) Hijacking Exploitation - Bypassing Non-Executable Stack Protection

Demonstrates the exploitation of Global Offset Table (GOT) hijacking techniques to bypass Non-Executable (NX) stack protection. The exploit leverages the writable nature of GOT entries under partial RELRO to redirect legitimate function calls to malicious targets, achieving arbitrary code execution and spawning a shell. Includes analysis of PLT/GOT dynamic linking mechanisms, manual GOT overwrite exploitation, and demonstration of both legitimate and malicious update processes using GDB and memory manipulation techniques. Shows how function pointer redirection bypasses NX protection by leveraging the program's existing executable code segments.

**Location:** `assignments/hw08_got_hijacking/`

**Note:** Requires 32-bit compilation support (`gcc-multilib` package) and GDB with pwndbg. ⚠️ **Security Warning:** This assignment demonstrates advanced GOT hijacking techniques. **MUST** be run in an isolated VM environment. Requires ASLR to be disabled and RELRO disabled (`-Wl,-z,norelro`) for demonstration. See assignment README for setup instructions.

## Requirements

- GCC compiler
- Make
- GDB (for debugging, pwndbg recommended)
- Python 2 (for hw06 exploit script)
- Python 3 with pwntools and ropper (for hw07)
- readelf and objdump (for hw08)
- For hw02, hw03, hw04, hw05, hw06, hw07, and hw08: 32-bit compilation support (install `gcc-multilib` on Debian/Ubuntu or `glibc-devel.i686 gcc-c++-i686` on Fedora)

## License

MIT License - see `LICENSE` file for details.
