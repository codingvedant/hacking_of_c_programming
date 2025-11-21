# Homework 03: Memory Layout and Debugging through Variable Declarations

**Author:** Vedant Bhalerao (vedantb@umd.edu)  
**Course:** ENPM691

## Overview

This experiment explores memory segmentation in C programs by analyzing the storage of variables from different storage classes. The program prints memory addresses of local, global, static, and dynamically allocated variables to demonstrate their placement in distinct memory segments: the stack, data, BSS, and heap. By correlating these observations with assembly code and debugger output, we verify how the compiler manages initialized versus uninitialized data.

## Building

**Note:** This assignment requires 32-bit compilation. You may need to install 32-bit libraries:
- On Debian/Ubuntu: `sudo apt-get install gcc-multilib`
- On Fedora: `sudo dnf install glibc-devel.i686 gcc-c++-i686`

```bash
make              # Build address_layout program
make run          # Build and run the program
make debug        # Build and run with GDB debugger
make clean        # Remove compiled binary
```

## Program

- `address_layout` - Prints memory addresses of variables from different storage classes

## Running

After building, run the program:

```bash
./address_layout
```

The program will print addresses for:
- Local (automatic) variables - stored on the stack
- Heap variables - dynamically allocated with malloc
- Global initialized variables - stored in .data segment
- Global uninitialized variables - stored in .bss segment
- Static local variables - stored in .data segment

## Debugging with GDB

To examine memory layout and assembly code:

```bash
gdb ./address_layout
(gdb) break main
(gdb) run
(gdb) info registers
(gdb) info variables
(gdb) disassemble main
(gdb) continue
```

Use `info variables` to see global and static variables in .data and .bss segments, and examine the stack with `x/20x $esp`.

## Compilation Flags

The Makefile uses:
- `-m32` - 32-bit object code for IA-32 stack frame analysis
- `-g` - Debug symbols for source-level inspection

## Expected Results

The program demonstrates that:
- **Stack variables** (local) appear at high addresses (e.g., 0xffcca5xx)
- **Heap variables** (malloc) appear in dynamic memory region (e.g., 0x56830xxx)
- **Global/Static variables** appear in lower addresses in .data/.bss segments (e.g., 0x565ce0xx)

See `docs/` folder for detailed write-up and analysis.

