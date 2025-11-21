# Homework 01: Memory Allocation and Alignment in C

**Author:** Vedant Bhalerao (vedantb@umd.edu)  
**Course:** ENPM691

## Overview

This assignment studies how the C compiler provides addresses to automatic variables, how to measure variable sizes, and how padding affects their alignment on the stack. The exercise demonstrates three approaches to variable declaration:

1. **Sequential Declaration** - Variables declared in order
2. **Reordered Declaration** - Variables reordered to reduce padding
3. **Packed Structure** - Using `__attribute__((packed))` to eliminate padding

## Building

```bash
make              # Build all three programs
make run          # Build and run all programs
make debug        # Build and run with GDB debugger
make clean        # Remove compiled binaries
```

## Programs

- `hw1_sequential` - Base program with sequential variable declaration
- `hw1_reordered` - Variables reordered to reduce padding
- `hw1_packed` - Variables in a packed structure

## Running

After building, run each program individually:

```bash
./hw1_sequential
./hw1_reordered
./hw1_packed
```

## Expected Results

The programs print memory addresses of variables, allowing you to:
- Calculate variable sizes from address differences
- Observe compiler-inserted padding
- Compare memory efficiency between approaches

See `docs/` folder for detailed write-up and analysis.

