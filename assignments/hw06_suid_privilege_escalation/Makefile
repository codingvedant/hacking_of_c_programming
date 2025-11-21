# Makefile for hw6 - builds 32-bit SUID binary with executable stack
# for privilege escalation exploitation experiments

CC = gcc
CFLAGS = -m32 -g -fno-stack-protector -z execstack -no-pie -mpreferred-stack-boundary=2
TARGET = shell_code
PYTHON = python2

all: $(TARGET)

$(TARGET): src/shell_code.c
	$(CC) $(CFLAGS) src/shell_code.c -o $(TARGET)

# Setup SUID privileges (requires root)
setup-suid: $(TARGET)
	@echo "Setting up SUID privileges (requires sudo):"
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)
	@echo "SUID setup complete. Binary now has root privileges."

# Remove SUID privileges
remove-suid: $(TARGET)
	@echo "Removing SUID privileges:"
	sudo chown $$USER:$$USER $(TARGET)
	sudo chmod u-s $(TARGET)
	@echo "SUID privileges removed."

run: $(TARGET)
	./$(TARGET) "test"

exploit: $(TARGET)
	@echo "Running exploit (requires SUID setup):"
	./$(TARGET) $$($(PYTHON) src/suidexploit.py)

debug: $(TARGET)
	gdb -q ./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all setup-suid remove-suid run exploit debug clean

