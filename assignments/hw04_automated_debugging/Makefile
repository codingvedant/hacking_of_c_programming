# Makefile for hw4 - builds 32-bit debug binary
# with optimizations disabled for memory layout analysis

CC = gcc
CFLAGS = -m32 -g -O0
TARGET = address_layout
GDB_SCRIPT = src/auto_script.gdb

all: $(TARGET)

$(TARGET): src/address_layout.c
	$(CC) $(CFLAGS) src/address_layout.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

auto-debug: $(TARGET)
	gdb -x $(GDB_SCRIPT) ./$(TARGET)

clean:
	rm -f $(TARGET) *.o address_layout_32_simple_output.txt

.PHONY: all run debug auto-debug clean

