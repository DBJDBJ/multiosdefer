# Compiler and flags
CC = gcc-14
CFLAGS = -std=c23 -Wall -Wextra -pedantic
DBGFLAGS = -g -O0

# Target executable name
TARGET = multiosdefer

# Source files
SRC = main.c

# Header files (for dependency tracking)
HEADERS = defer_gcc.h msvc_defer_we_preffer.h abandoned_defer_msvc.h

# Default target: build release version
all: $(TARGET)

# Build release version
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Build debug version with symbols
debug: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(DBGFLAGS) -o $(TARGET) $(SRC)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Build and run
run: $(TARGET)
	./$(TARGET)

# Phony targets (not actual files)
.PHONY: all clean run debug
