# Homework 05: Executing Shellcode via Return-Address Overwrite and Function-Pointer Call

**Author:** Vedant Bhalerao (vedantb@umd.edu)  
**Course:** ENPM691

## Overview

This assignment explores low-level program control and stack memory manipulation in C by implementing and executing two shellcode-based examples, shell1.c and shell2.c. Both programs embed 32-bit assembly instructions to invoke the exit(2) system call, demonstrating two different methods to transfer execution to shellcode: overwriting the return address and using a function pointer. Experiments are performed under GDB with the pwndbg extension to observe runtime behavior, memory layout, and instruction execution.

> ⚠️ **Warning:** This assignment demonstrates exploitation techniques for educational purposes only. These programs are designed to run in a controlled lab environment with security mitigations disabled. Do not use these techniques on systems you do not own or have explicit permission to test.

## Building

**Note:** This assignment requires 32-bit compilation. You may need to install 32-bit libraries:
- On Debian/Ubuntu: `sudo apt-get install gcc-multilib`
- On Fedora: `sudo dnf install glibc-devel.i686 gcc-c++-i686`

```bash
make              # Build shell1 and shell2
make run          # Build and run both programs
make debug        # Build and run shell1 with GDB debugger
make clean        # Remove compiled binaries
```

## Programs

- **shell1** - Executes shellcode by overwriting the return address
- **shell2** - Executes shellcode via function pointer call

## Running

After building, run each program:

```bash
./shell1
echo "exit code: $?"  # Expected: 2

./shell2
echo "exit code: $?"  # Expected: 2
```

Both programs should exit with code 2, confirming that the shellcode executed successfully.

## Debugging with GDB

To examine the shellcode execution:

**For shell1:**
```bash
gdb -q ./shell1
(gdb) break main
(gdb) run
(gdb) info registers
(gdb) disassemble main
(gdb) vmmap
(gdb) continue
```

**For shell2:**
```bash
gdb -q ./shell2
(gdb) break main
(gdb) run
(gdb) info registers
(gdb) disassemble main
(gdb) vmmap
(gdb) continue
```

Use `vmmap` to verify the stack is executable (should show `rwx` permissions).

## Compilation Flags

The Makefile uses specific flags to create a controlled lab environment:
- `-m32` - 32-bit (IA-32) executable
- `-g` - Debug symbols for source-level debugging
- `-fno-stack-protector` - Disable stack canaries
- `-no-pie` - Disable position-independent executable
- `-z execstack` - Mark stack as executable (required for shellcode execution)

## Shellcode Details

Both programs use the same shellcode that calls `exit(2)`:
```assembly
mov eax, 1    ; syscall number for exit
mov ebx, 2    ; exit status code
int 0x80      ; invoke system call
```

This provides a safe, observable indicator that injected code executed without spawning an interactive shell.

## Execution Methods

1. **shell1 (Return Address Overwrite)**: 
   - Stores shellcode on the stack
   - Overwrites the saved return address to point to shellcode
   - When main returns, execution jumps to shellcode

2. **shell2 (Function Pointer Call)**:
   - Stores shellcode on the stack
   - Casts shellcode address to function pointer
   - Directly calls the function pointer

## Expected Results

Both programs should:
- Execute the shellcode successfully
- Exit with status code 2
- Demonstrate different methods of transferring execution to injected code

See `docs/` folder for detailed write-up and analysis.

## Security Notes

These programs demonstrate exploitation techniques in a controlled environment. In production systems, modern security mitigations would prevent these attacks:
- **NX/DEP** - Prevents execution of stack memory
- **ASLR** - Randomizes memory addresses
- **Stack Canaries** - Detects stack buffer overflows
- **PIE** - Makes code addresses unpredictable

