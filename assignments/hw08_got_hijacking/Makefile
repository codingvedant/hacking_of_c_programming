# Makefile for hw8 - builds 32-bit binary for GOT hijacking exploitation
# Demonstrates bypassing NX protection using GOT manipulation

CC = gcc
CFLAGS = -m32 -fno-stack-protector -no-pie -mpreferred-stack-boundary=2 -fno-pic
LDFLAGS = -Wl,-z,norelro
TARGET = got

all: $(TARGET)

$(TARGET): src/got.c
	$(CC) $(CFLAGS) $(LDFLAGS) src/got.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb -q ./$(TARGET)

# Disable ASLR (requires root)
disable-aslr:
	@echo "Disabling ASLR (requires sudo):"
	sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"
	@echo "ASLR disabled. Current value:"
	cat /proc/sys/kernel/randomize_va_space

# Re-enable ASLR (requires root)
enable-aslr:
	@echo "Re-enabling ASLR (requires sudo):"
	sudo sh -c "echo 2 > /proc/sys/kernel/randomize_va_space"
	@echo "ASLR enabled. Current value:"
	cat /proc/sys/kernel/randomize_va_space

# Setup malicious script in PATH
setup-script:
	@echo "Setting up malicious 'Your' script:"
	@mkdir -p $$HOME/.local/bin
	@cp src/Your.sh $$HOME/.local/bin/Your
	@chmod +x $$HOME/.local/bin/Your
	@echo "Script created at: $$HOME/.local/bin/Your"
	@echo "Make sure $$HOME/.local/bin is in your PATH"

# Remove malicious script
remove-script:
	@echo "Removing malicious script:"
	@rm -f $$HOME/.local/bin/Your
	@echo "Script removed"

# Check security settings
checksec: $(TARGET)
	@echo "Checking binary security settings:"
	checksec $(TARGET) 2>/dev/null || echo "checksec not available, use: readelf -l $(TARGET) | grep RELRO"

# Show GOT/PLT information
show-got: $(TARGET)
	@echo "GOT/PLT information:"
	@readelf -r $(TARGET) | head -20
	@echo "\nPLT section:"
	@objdump -d -j .plt $(TARGET) | head -30

clean:
	rm -f $(TARGET) *.o

.PHONY: all run debug disable-aslr enable-aslr setup-script remove-script checksec show-got clean

