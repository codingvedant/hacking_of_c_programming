# Makefile for hw5 - builds 32-bit binaries with executable stack
# for shellcode execution experiments

CC = gcc
CFLAGS = -m32 -g -fno-stack-protector -no-pie -z execstack
TARGETS = shell1 shell2

all: $(TARGETS)

shell1: src/shell1.c
	$(CC) $(CFLAGS) src/shell1.c -o shell1

shell2: src/shell2.c
	$(CC) $(CFLAGS) src/shell2.c -o shell2

run: all
	@echo "Running shell1:"
	./shell1
	@echo "exit code: $$?"
	@echo "\nRunning shell2:"
	./shell2
	@echo "exit code: $$?"

debug: shell1
	gdb -q ./shell1

clean:
	rm -f $(TARGETS) *.o

.PHONY: all run debug clean

