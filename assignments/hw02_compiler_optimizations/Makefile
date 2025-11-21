# Makefile for hw2 - builds 32-bit debug binaries
# with no PIE, no stack protectors

CC = gcc
CFLAGS = -m32 -g -fno-stack-protector -no-pie -fno-pic -z execstack

SRCS = src/mulby3.c src/mulby12.c src/mulby7.c
BINS = mulby3 mulby12 mulby7

all: $(BINS)

mulby3: src/mulby3.c
	$(CC) $(CFLAGS) src/mulby3.c -o mulby3

mulby12: src/mulby12.c
	$(CC) $(CFLAGS) src/mulby12.c -o mulby12

mulby7: src/mulby7.c
	$(CC) $(CFLAGS) src/mulby7.c -o mulby7

run: all
	@echo "Running mulby3:"
	./mulby3
	@echo "\nRunning mulby7:"
	./mulby7
	@echo "\nRunning mulby12:"
	./mulby12

debug: mulby3
	gdb ./mulby3

clean:
	rm -f $(BINS) *.o

.PHONY: all run debug clean

