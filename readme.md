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

## Requirements

- GCC compiler
- Make
- GDB (for debugging)

## License

MIT License - see `LICENSE` file for details.
