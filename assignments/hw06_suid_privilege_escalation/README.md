# Homework 06: Stack-Based Privilege Escalation via JMP %ESP Buffer Overflow Exploitation

**Author:** Vedant Bhalerao (vedantb@umd.edu)  
**Course:** ENPM691

## Overview

This assignment explores stack-based buffer overflow exploitation and privilege escalation in Linux by implementing and executing a SUID shell acquisition through the JMP %ESP technique. The program shell_code.c contains a deliberate buffer overflow vulnerability that is exploited to execute injected shellcode with elevated privileges. Experiments are performed under GDB with the pwndbg extension to analyze memory layout, identify the JMP %ESP gadget, and verify successful code execution.

> ⚠️ **CRITICAL WARNING:** This assignment demonstrates privilege escalation exploitation techniques for educational purposes only. These programs are designed to run in a controlled, isolated lab environment (VM) with security mitigations disabled. **NEVER** run these programs on production systems or systems you do not own. See `SAFE_USAGE.md` in the repository root.

## Building

**Note:** This assignment requires 32-bit compilation. You may need to install 32-bit libraries:
- On Debian/Ubuntu: `sudo apt-get install gcc-multilib`
- On Fedora: `sudo dnf install glibc-devel.i686 gcc-c++-i686`

```bash
make              # Build shell_code binary
make setup-suid    # Set SUID root privileges (requires sudo)
make exploit      # Run the exploit
make remove-suid  # Remove SUID privileges when done
make debug        # Run with GDB debugger
make clean        # Remove compiled binaries
```

## Programs

- **shell_code** - Vulnerable SUID program with buffer overflow
- **suidexploit.py** - Python exploit script that constructs the payload

## Setup Instructions

1. **Build the binary:**
   ```bash
   make
   ```

2. **Set SUID privileges (requires root):**
   ```bash
   make setup-suid
   # or manually:
   sudo chown root:root shell_code
   sudo chmod u+s shell_code
   ```

3. **Verify SUID is set:**
   ```bash
   ls -l shell_code
   # Should show: -rwsr-xr-x (the 's' indicates SUID)
   ```

## Running the Exploit

After setting up SUID privileges, run the exploit:

```bash
make exploit
# or manually:
./shell_code $(python2 src/suidexploit.py)
```

If successful, you should get a root shell:
```bash
$ ./shell_code $(python2 src/suidexploit.py)
# whoami
root
# id
uid=0(root) gid=0(root) groups=0(root)
# exit
```

## Debugging with GDB

To examine the exploitation process:

```bash
gdb -q ./shell_code
(gdb) break main
(gdb) run
(gdb) print jmpesp
# Should show: $1 = {void (void)} 0x8049166 <jmpesp>
(gdb) disassemble main
(gdb) continue
```

Use `vmmap` in pwndbg to verify the stack is executable.

## Exploit Details

The exploit uses the JMP %ESP technique:

1. **Padding (88 bytes)**: Fills the buffer to reach the saved return address
2. **JMP %ESP address (4 bytes)**: Overwrites the return address with the gadget address (0x8049166)
3. **NOP sled (8 bytes)**: Provides alignment buffer
4. **Shellcode**: SUID-aware shellcode that spawns a root shell

The JMP %ESP gadget (`jmpesp()` function) redirects execution to the stack, where the shellcode is located.

## Compilation Flags

The Makefile uses specific flags to create a controlled lab environment:
- `-m32` - 32-bit (IA-32) executable
- `-g` - Debug symbols for source-level debugging
- `-fno-stack-protector` - Disable stack canaries
- `-z execstack` - Mark stack as executable (required for shellcode)
- `-no-pie` - Disable position-independent executable
- `-mpreferred-stack-boundary=2` - Control stack alignment

## Security Notes

This assignment demonstrates exploitation techniques in a controlled environment. In production systems, modern security mitigations would prevent these attacks:

- **NX/DEP** - Prevents execution of stack memory
- **ASLR** - Randomizes memory addresses (would break fixed gadget addresses)
- **Stack Canaries** - Detects stack buffer overflows
- **PIE** - Makes code addresses unpredictable
- **SUID restrictions** - Modern systems have additional SUID protections

**Always remove SUID privileges when done:**
```bash
make remove-suid
```

See `docs/` folder for detailed write-up and analysis.

