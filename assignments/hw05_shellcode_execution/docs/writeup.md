# ENPM691 Homework Assignment 05: Executing Shellcode via Return-Address Overwrite and Function-Pointer Call

**Vedant Bhalerao**  
vedantb@umd.edu

## Abstract

This assignment explores low-level program control and stack memory manipulation in C by implementing and executing two shellcode-based examples, shell1.c and shell2.c. Both programs embed 32-bit assembly instructions to invoke the exit(2) system call, demonstrating two different methods to transfer execution to shellcode: overwriting the return address and using a function pointer. Experiments were performed under GDB with the pwndbg extension to observe runtime behavior, memory layout, and instruction execution. Disassembly and debugger snapshots confirm how the stack and executable memory regions are used to run injected code. The results illustrate how memory layout, compiler-generated stack frames, and system call conventions interact in low-level C programming, providing practical understanding of stack exploitation, code injection, and process control.

**Index Terms:** Stack exploitation, Shellcode, Return address overwrite, Function pointer call, Executable stack, NX/DEP, ASLR, Control-flow hijacking, System calls, int 0x80, Code injection, GDB debugging, pwndbg, C programming, Low-level debugging, Security mitigations

## I. Introduction

Buffer overflow vulnerabilities remain a critical security concern where improper input validation allows attackers to overwrite stack memory and hijack program execution. This assignment demonstrates two practical exploitation methods to achieve shell execution on vulnerable systems.

Method 1 employs traditional shellcode injection through buffer overflow, redirecting execution to attacker-controlled code. Method 2 uses function pointer manipulation to directly execute shellcode stored in memory. Both approaches leverage the absence of stack protection mechanisms to bypass security controls.

We utilize GDB with pwndbg extensions to automate stack analysis, offset calculation, and payload testing. This scripted approach ensures consistent experimentation and reproducible results. The investigation connects theoretical security concepts with hands-on exploitation, providing insights into both attack techniques and essential defensive programming practices for secure software development.

## II. Methodology

To examine how stack-resident shellcode is placed and executed in two minimal programs (shell1.c and shell2.c), a reproducible compile–run–debug workflow was adopted. All experiments were performed on a Kali Linux host using the GNU Debugger (gdb) with the pwndbg extension to facilitate memory inspection and capture consistent artifacts for the Results and Appendix.

### System Information:

- Operating System: Kali Linux 6.12.38+kali-amd64
- Kernel Version: 6.12.38-1kali1
- Architecture: x86 (IA-32 / 32-bit mode)
- Compiler: GCC (GNU Compiler Collection)
- GCC Version: 14.3.0 (Debian 14.3.0-5)
- Debugger: GDB with pwndbg

### A. Overview

The two programs under study are minimal examples that demonstrate execution of machine code placed in an automatic (stack) buffer:

- **shell1.c** — places shellcode bytes in a local array and demonstrates execution by redirecting control flow toward that stack buffer (consistent with return-address redirection techniques).
- **shell2.c** — places identical shellcode in a local array and invokes it by casting the array address to a function pointer and calling it directly.

Both programs use the same small proof-of-execution shellcode that calls exit(2) with status code 2; this choice provides a safe, observable indicator that injected code executed without spawning an interactive shell.

### B. Compilation

Binaries were built for 32-bit execution with debugging symbols and with selected mitigations disabled to create a controlled lab environment suitable for observation:

- `-m32`: build 32-bit (IA-32) executable.
- `-g`: include debug symbols for source-level debugging.
- `-fno-stack-protector`: disable stack canaries for lab observation.
- `-Wl,-no-pie` (or `-no-pie`): disable PIE to keep code addresses stable.
- `-z execstack`: request an executable stack segment (allows execution of stack-resident code).

### C. Execution Verification

Each binary was executed outside the debugger to verify behavior and capture the observable result from the embedded shellcode:

```bash
./shell1
echo "exit code: $?" # expected: 2

./shell2
echo "exit code: $?" # expected: 2
```

### D. Interactive Debugging Procedure (manual gdb/pwndbg)

All debugger work was performed manually using gdb with the pwndbg extension. The following is the exact interactive procedure followed for each binary; these commands were used to collect the snapshots and screenshots included in the Results and Appendix.

1. Launch GDB with the target binary:
   ```
   gdb -q ./shell1
   # or: gdb -q ./shell2
   ```

2. Set a breakpoint at main and run the program to pause at the function entry:
   ```
   break main
   run
   ```

3. Inspect the program context (registers, nearby disassembly, and a short stack view):
   ```
   context
   info registers
   ```

4. View the process memory map and confirm segment permissions (to show whether the stack is executable):
   ```
   vmmap
   checksec # pwndbg convenience command, or use external checksec output
   ```

## III. Results

All experiments were performed with the same compilation flags and executed under gdb using the pwndbg extension. The purpose was to execute shellcode to call exit(2) using two different methods. Runtime addresses, disassembly, and debugger observations are summarized below.

### A. Shell1: Overwriting Return Address

- **Description:** Shellcode bytes were stored on the stack, and the saved return address of main was overwritten to point to the shellcode.

- **Runtime Output:**
  ```
  $ ./shell1
  $ echo "exit code: $?"
  exit code: 2
  ```

- **Disassembly (main function):**
  ```
  0x08049166 <+16>: mov DWORD PTR [ebp-0xd],0x1b8
  0x0804916d <+23>: mov DWORD PTR [ebp-0x9],0x2bb00
  0x08049174 <+30>: mov DWORD PTR [ebp-0x5],0x80cd0000
  0x0804917b <+37>: mov BYTE PTR [ebp-0x1],0x0
  0x0804917f <+41>: lea eax,[ebp-0x14]
  0x08049182 <+44>: add eax,0x18
  0x08049185 <+47>: mov DWORD PTR [ebp-0x14],eax
  0x08049188 <+50>: mov eax,DWORD PTR [ebp-0x14]
  0x0804918b <+53>: lea edx,[ebp-0xd]
  0x0804918e <+56>: mov DWORD PTR [eax],edx
  0x08049190 <+58>: mov eax,0x0
  0x08049195 <+63>: leave
  0x08049196 <+64>: ret
  ```

- **Debugger Snapshot:** Stack inspection confirmed the shellcode was stored at offsets near EBP-0xd and the return address was successfully overwritten to point to it. Execution jumped to the shellcode, which terminated the program with exit code 2.

### B. Shell2: Function Pointer Call

- **Description:** Shellcode was stored on the stack and executed by assigning its address to a function pointer, then calling it directly.

- **Runtime Output:**
  ```
  $ ./shell2
  $ echo "exit code: $?"
  exit code: 2
  ```

- **Disassembly (main function):**
  ```
  0x08049169 <+19>: mov DWORD PTR [esp+0xf],0x1b8
  0x08049171 <+27>: mov DWORD PTR [esp+0x13],0x2bb00
  0x08049179 <+35>: mov DWORD PTR [esp+0x17],0x80cd0000
  0x08049181 <+43>: mov BYTE PTR [esp+0x1b],0x0
  0x08049186 <+48>: lea eax,[esp+0xf]
  0x0804918a <+52>: mov DWORD PTR [esp+0x1c],eax
  0x0804918e <+56>: mov eax,DWORD PTR [esp+0x1c]
  0x08049192 <+60>: call eax
  0x08049194 <+62>: leave
  0x08049195 <+63>: ret
  ```

- **Debugger Snapshot:** The stack contains the shellcode, and the function pointer correctly points to it. Execution jumps to the shellcode, which runs the exit(2) syscall and terminates the program.

### C. Summary Table of Observed Execution Techniques

| Method | Code Example | Mechanism |
|--------|--------------|-----------|
| Shell1 | Overwriting return address | Writes shellcode to stack, modifies saved return address to jump to shellcode |
| Shell2 | Function pointer call | Stores shellcode on stack and calls via function pointer |

## IV. Discussion

This section analyzes the two stack-hacking techniques implemented in shell1 and shell2, relates the observed debugger output and disassembly to the underlying CPU actions, and discusses security implications and portability concerns. Both programs were compiled as 32-bit binaries with stack execution enabled:

### A. Mechanics of shell1 (return address overwrite)

shell1 implements the "overwrite saved return address" technique. The disassembly excerpt (see Results) shows the compiler writing the literal shellcode bytes into the stack frame (instructions such as `mov DWORD PTR [ebp-0xd],0x1b8`, `mov DWORD PTR [ebp-0x9],0x2bb00`, `mov DWORD PTR [ebp-0x5],0x80cd0000`, etc.). These immediates, when concatenated in memory, form the machine code sequence:

```
b8 01 00 00 00 bb 02 00 00 00 cd 80
```

which correspond to the 32-bit instructions `mov eax,1; mov ebx,2; int 0x80` (the `exit(2)` syscall).

The subsequent instruction sequence computes the target stack address and writes the shellcode address into that return-slot. At function return, the saved EIP is popped from the stack and control flows to the shellcode. The shellcode executes `int 0x80` with `eax=1` and `ebx=2`, causing the process to exit with status 2; this is verified by the observed `exit code: 2`.

### B. Mechanics of shell2 (function pointer call)

shell2 demonstrates the second technique: store the shellcode on the stack and directly call it via a function pointer. The disassembly shows the compiler placing identical shellcode bytes into the stack and then loading the shellcode address into a register and invoking `call eax`:

```
lea eax,[esp+0xf]
mov DWORD PTR [esp+0x1c],eax
mov eax,DWORD PTR [esp+0x1c]
call eax
```

This is effectively equivalent at runtime to:
```c
int (*shell)(void) = (void *)shellcode;
shell();
```

Because the code performs an explicit indirect call, it does not depend on corrupting saved control-flow metadata (saved return address) — rather it relies on the ability to execute code at the stack address. The observed exit code again validates that the `int 0x80` in the shellcode executed.

### C. Comparison of the two methods

- **Complexity:** The function-pointer method (shell2) is conceptually simpler and less fragile: it does not need to compute or overwrite a saved return address. The return-overwrite method (shell1) demonstrates how corrupting control-flow metadata leads to redirection of execution.

- **Detectability:** Overwriting return addresses is more likely to be detected by stack canaries and by control-flow integrity tools. Direct function-pointer invocation is simpler to reason about but still prevented by non-executable memory.

### D. Conclusion

The two exercises demonstrate two canonical methods for transferring execution to injected code on the stack: overwriting a saved return address and calling a stack-resident function pointer. The experiments validate the expected runtime behavior (confirmed by gdb disassembly and the observed exit code: 2). They also clearly illustrate why modern defensive features (NX, ASLR, stack canaries, and PIE) are effective at preventing or complicating such attacks. These controlled experiments deepen understanding of how storage, calling conventions, and processor privilege interact in real exploitation scenarios.

## V. Appendix

### Appendix A: Source files

See `src/` directory for:
- `shell1.c` - Return address overwrite method
- `shell2.c` - Function pointer call method

### Appendix B: Makefile

See `Makefile` in this directory.

### Appendix C: GDB Commands

**For shell1:**
```
gdb -q ./shell1
break main
run
info registers
disassemble main
vmmap
quit
```

**For shell2:**
```
gdb -q ./shell2
break main
run
info registers
disassemble main
vmmap
quit
```

## References

[1] R. Bryant and D. O'Hallaron, Computer Systems: A Programmer's Perspective, 3rd ed. Boston, MA: Pearson, 2016.

[2] E. Aleph One, "Smashing the stack for fun and profit," Phrack Magazine, Issue 49, 1996.

[3] Free Software Foundation, GDB: The GNU Project Debugger. [Online]. Available: https://www.gnu.org/software/gdb/.

[4] Pwndbg Team, Pwndbg: GDB plugin for exploit development. [Online]. Available: https://github.com/pwndbg/pwndbg.

