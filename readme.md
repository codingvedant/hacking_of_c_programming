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

## Requirements

- GCC compiler
- Make
- GDB (for debugging)
- For hw02, hw03, and hw04: 32-bit compilation support (install `gcc-multilib` on Debian/Ubuntu or `glibc-devel.i686 gcc-c++-i686` on Fedora)

## License

MIT License - see `LICENSE` file for details.
