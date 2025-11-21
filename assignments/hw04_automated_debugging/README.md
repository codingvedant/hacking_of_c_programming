# Homework 04: Memory Layout and Automated Debugging with Variable Declarations

**Author:** Vedant Bhalerao (vedantb@umd.edu)  
**Course:** ENPM691

## Overview

This assignment investigates how C programs organize variables of different storage classes in memory. Using the program address_layout.c, we examine the runtime addresses of local, global (initialized and uninitialized), static, and dynamically allocated variables. By combining program output with automated inspection using a GDB script and the pwndbg extension, we map each variable type to its corresponding memory segment: stack, heap, data, and BSS. The analysis also includes disassembled code to illustrate how the compiler accesses variables based on their storage class.

## Building

**Note:** This assignment requires 32-bit compilation. You may need to install 32-bit libraries:
- On Debian/Ubuntu: `sudo apt-get install gcc-multilib`
- On Fedora: `sudo dnf install glibc-devel.i686 gcc-c++-i686`

```bash
make              # Build address_layout program
make run          # Build and run the program
make debug        # Build and run with GDB debugger (interactive)
make auto-debug   # Run automated GDB script
make clean        # Remove compiled binary and output files
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

## Automated Debugging

This assignment emphasizes automated debugging using a GDB script. The script (`src/auto_script.gdb`) automatically:

- Sets breakpoints and runs the program
- Displays process memory mappings
- Prints addresses of all variables
- Disassembles the main function
- Dumps heap memory contents
- Lists all global and static symbols
- Logs all output to `address_layout_32_simple_output.txt`

To run the automated script:

```bash
make auto-debug
```

Or manually:

```bash
gdb -x src/auto_script.gdb ./address_layout
```

## Interactive Debugging with GDB

For manual inspection:

```bash
gdb ./address_layout
(gdb) break main
(gdb) run
(gdb) info registers
(gdb) info variables
(gdb) info proc mappings
(gdb) disassemble main
(gdb) continue
```

## Compilation Flags

The Makefile uses:
- `-m32` - 32-bit object code for IA-32 stack frame analysis
- `-g` - Debug symbols for source-level inspection
- `-O0` - Disable optimizations to preserve original variable layout

## Expected Results

The program demonstrates that:
- **Stack variables** (local) appear at high addresses (e.g., 0xffc9d5xx)
- **Heap variables** (malloc) appear in dynamic memory region (e.g., 0x57f2cxxx)
- **Global/Static variables** appear in lower addresses in .data/.bss segments (e.g., 0x565bd0xx)

The automated GDB script will generate `address_layout_32_simple_output.txt` with all debugger output for analysis.

See `docs/` folder for detailed write-up and analysis.

