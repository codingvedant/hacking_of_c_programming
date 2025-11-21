CC = gcc
CFLAGS = -g -Wall
TARGETS = hw1_sequential hw1_reordered hw1_packed

all: $(TARGETS)

hw1_sequential: src/hw1_sequential.c
	$(CC) $(CFLAGS) -o hw1_sequential src/hw1_sequential.c

hw1_reordered: src/hw1_reordered.c
	$(CC) $(CFLAGS) -o hw1_reordered src/hw1_reordered.c

hw1_packed: src/hw1_packed.c
	$(CC) $(CFLAGS) -o hw1_packed src/hw1_packed.c

run: all
	@echo "Running sequential declaration:"
	./hw1_sequential
	@echo "\nRunning reordered declaration:"
	./hw1_reordered
	@echo "\nRunning packed structure:"
	./hw1_packed

debug: hw1_sequential
	gdb ./hw1_sequential

clean:
	rm -f $(TARGETS) *.o

.PHONY: all run debug clean

