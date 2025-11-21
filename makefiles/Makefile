# Shared Makefile for ENPM691 assignments
# This can be included in assignment-specific Makefiles if needed

CC = gcc
CFLAGS = -g -Wall -Wextra
LDFLAGS = 

# Common targets that can be overridden
.PHONY: all clean help

help:
	@echo "Available targets:"
	@echo "  all    - Build all targets"
	@echo "  clean  - Remove compiled binaries"
	@echo "  help   - Show this help message"

# Default clean target
clean:
	rm -f *.o *.exe *.out

