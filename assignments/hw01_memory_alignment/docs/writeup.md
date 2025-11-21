# ENPM691 Homework Assignment 01: Memory Allocation and Alignment in C

**Vedant Bhalerao**  
vedantb@umd.edu

## Abstract

This lab studies how the C compiler provides addresses to automatic variables, how to measure variable sizes, and how padding affects their alignment on the stack. Common variable types were declared and analyzed in three ways: basic sequential declaration, reordering of variables to reduce packing and using a packed structure to remove this padding wholly. The results show that compiler introduces padding to improve memory access speeds but also create gaps between variables, causing discrepancy while calculating their sizes. While reordering these variables reduces some padding, converting them into packed structures removes the padding entirely, making it more compact. These observations highlight the importance of understanding memory alignment for writing efficient C programs.

**Index Terms:** C programming, memory alignment, stack allocation, padding, packed structures, compiler optimization, data organization, variable reordering

## I. Introduction

When a C program is executed, each of the local variables that are declared are put on a stack. Each variable is allocated memory as soon as it declared. The stack organizes these variables in contiguous locations, and the space allocated to variables are dependent on the compiler and system architecture. While these sizes are fixed according to the architecture, compilers add padding to these variables for efficiently accessing memory and increasing performance. The purpose of this assignment is to set up a working environment, develop C programs and investigate how variable sizes and alignment are done in memory. We collect the addresses of different types of variables and compute their sizes using their memory addresses, identify any padding involved and confirm their true sizes. This provides practical insight into low-level memory management and how it affects the system performance.

## II. Methodology

In order to compare memory sizes and alignment for variables, a sequence of C programs was written and executed. The programs were compiled alike using gcc on a Linux environment, and output was observed both by printing addresses and by using the debugger. The exercise was divided into three phases:

### A. Base program with sequential declaration

- A simple program was written with local automatic variables of types int, char, float, long, double, and pointer.
- This shows that variables are stored in the stack based on the declaration order but alignment requirements introduce padding between some variables.

### B. Reordered declaration to reduce padding

- The variables were re-declared by first declaring larger data types (such as double, long) and later smaller ones (such as char).
- This arrangement reduced unnecessary memory holes, yet there was still padding due to strict alignment requirements, especially for pointer and double types.
- The results confirmed that ordering can reduce memory usage, but alignment cannot be avoided.

### C. Using Packed Structure to eliminate padding

- A structure was declared to include all variable types in one place.
- The `__attribute__((packed))` directive was applied to the structure, forcing the compiler to pack the variables in order without padding.
- The addresses of the structure members showed that they were appearing sequentially one after the other, leaving no gaps in the alignment.

## III. Results

The tests calculated the variable sizes according to the memory spacing (from addresses printed). Three methods were tried: sequential declaration, reordered declarations, and a packed structure.

### A. Base program with sequential declaration

Variables of int, char, float, long, double, and pointer types were declared one after another. The printed addresses showed that while some of the variables occupied space equal to what we should expect according to the architecture, others showed unexpected gaps due to compiler-inserted padding. For example:

- var2 (int) and var3 (char) were 4 bytes and 1 byte apart respectively, as we would expect.
- Spacings between var3 (char) and var4 (float) was 7 bytes, where it should only be 4 bytes. This is because of added padding.
- Larger data types like long, pointer, and double were aligned on 8-byte boundaries as expected.

### B. Reordered declaration to reduce padding

Variables rearranged with larger data types first produced memory layout with fewer but still noticeable padding spaces. For example:

- There was a 12-byte gap between var1 (int) and var7 (double), indicating the compiler's effort to double-align the double on an 8-byte boundary.
- Other variable types matched the expected size.

This confirms that variable order can reduce wasted space, but padding cannot be entirely avoided since certain data types align strictly.

### C. Using Packed Struct to eliminate padding

- The variables were grouped into a struct with the `__attribute__((packed))` directive. The addresses output with no space between members:
- The address of each member was offset by exactly their expected size (char = 1, int = 4, float = 4, long = 8, pointer = 8, double = 8).
- This demonstrated that packing needs a compact memory space with no extra space added for alignment.

## IV. Discussions

The experiments illustrate how compilers handle storage of variables on the stack and how alignment influences memory organization. Some points to note from the results:

### A. Alignment vs. Sequential Allocation

Variables of int, char, float, long, double, and pointer types were declared one after another. The printed addresses showed that while some of the variables occupied space equal to what we should expect according to the architecture, others showed unexpected gaps due to compiler-inserted padding. For example:

- Variables are not necessarily stored one after another in memory even though they are declared one after another within a function. The compiler, however, insists on adherence to alignment requirements so that bigger types such as double, long, and pointers start at addresses that are multiples of 8.
- This is the reason why there is a discrepancy in the calculated sizes of variables, as the compiler inserted padding bytes to provide proper alignment.

### B. Effect of Reordering

When variables were ordered by large types first, gaps in alignment were reduced to a minimum, therefore confirming that variable ordering has an impact on memory efficiency. Alignment rules, however, still needed padding, which means although reordering does make a difference, it can't eliminate wasted space completely.

### C. Packed Structure behaviour

- The packed struct demonstrated a tightly packed one where the difference in the two consecutive addresses equaled the exact size of each type. This confirmed that padding had been removed.
- But while this method saves memory, it will do so at the cost of being slower or dangerous sometimes.

## V. Appendix

### Appendix A: Source Code - Sequential Declaration

See `src/hw1_sequential.c`

### Appendix B: Source Code - Reordered Declaration

See `src/hw1_reordered.c`

### Appendix C: Source Code - Packed Structure

See `src/hw1_packed.c`

### Appendix D: Makefiles / Scripts

We used the GCC compiler and GDB debugger to compile, run, and debug our C programs. See the `Makefile` in this directory.

