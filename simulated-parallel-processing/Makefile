# Compiler & flags
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -O2 -Iinclude

# Source files
MAIN_SRC = main.c io_helpers.c
PROCESS_SRC = process.c io_helpers.c

# Targets
all: main process

main:
	$(CC) $(CFLAGS) -o main $(MAIN_SRC)

process:
	$(CC) $(CFLAGS) -o process $(PROCESS_SRC)

clean:
	rm -f main process

.PHONY: all clean