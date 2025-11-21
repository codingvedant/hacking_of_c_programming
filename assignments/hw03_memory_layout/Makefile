# Makefile for hw3 - builds 32-bit debug binary
# for memory layout analysis

CC = gcc
CFLAGS = -m32 -g
TARGET = address_layout

all: $(TARGET)

$(TARGET): src/address_layout.c
	$(CC) $(CFLAGS) src/address_layout.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run debug clean

