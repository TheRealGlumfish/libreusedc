# Copyright (c) 2021, Dimitrios Alexopoulos All rights reserved.

# C compiler that will be used
CC = clang
# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -pedantic-errors -std=c18
# Compiler flags to be only used in the final linking stag
LINK_CFLAGS = -lgmp
# Additional compiler flags to be used in debug builds
DEBUG_CFLAGS = -g3 -glldb -fsanitize=address,undefined,integer,implicit-conversion -fno-omit-frame-pointer
# Additional flags to be used with release builds
RELEASE_CFLAGS = -O -DNDEBUG
# Directory which holds source code
SRC_DIR = src/
# Directory which holds header includes
INCLUDE_DIR = include/
# Directory which holds object files and other files used when building
BUILD_DIR = build/
# Directory which holds compiled (and linked) binaries
BIN_DIR = bin/
# Name of executable binary
BIN_NAME = libreusedc
# Directory which binary will be installed when running target install
INSTALL_DIR = /usr/local/
# clang-format flags
CLANG_FORMAT_FLAGS = --style="{$(CLANG_FORMAT_STYLE)}"
# clang-format style options
CLANG_FORMAT_STYLE = UseTab: Never, IndentWidth: 4, BreakBeforeBraces: Allman, AllowShortIfStatementsOnASingleLine: false, IndentCaseLabels: false, ColumnLimit: 0

.PHONY: all clean build install uninstall release test analyze compiledb format format-check todo note

all: debug

# Normal build with no debugging or optimizations
build: $(BIN_DIR)$(BIN_NAME)

# Release build with optimizations enabled
release: CFLAGS += $(RELEASE_CFLAGS)
release: clean build

# Debug build (special flags)
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: build

# Links object code to produce final executable
$(BIN_DIR)$(BIN_NAME): $(BUILD_DIR)numbers.o
	$(CC) $(CFLAGS) $(LINK_CFLAGS) $(BUILD_DIR)* -o $(BIN_DIR)$(BIN_NAME)

# Compiles all translation units into object code
$(BUILD_DIR)numbers.o: $(SRC_DIR)numbers.c
	$(CC) $(CFLAGS) $(SRC_DIR)numbers.c -c -o $(BUILD_DIR)numbers.o

# Generates debug symbols bundle
#dSYM:
#	dsymutil $(BIN_DIR)$(BIN_NAME)

# Cleans object file and binary directories
clean:
	-rm -r $(BIN_DIR)* $(BUILD_DIR)*

# Creates binary and object file directories
config:
	-mkdir $(BUILD_DIR) $(BIN_DIR)

# Runs unit tests
test:
 #TODO: Implement unit tests

# Generates the compile_commands.json using the bear tool
compiledb: compile_commands.json

compile_commands.json: Makefile
	make clean
	bear -- make debug

# Runs clang static analyzer
analyze: clean compiledb
	scan-build make debug

# Installs in bin folder
#install: config release
#	cp $(BIN_DIR)$(BIN_NAME) $(INSTALL_DIR)$(BIN_NAME)

# Uninstalls from bin folder
#remove:
#	rm $(INSTALL_DIR)$(BIN_NAME)

# Formats using clang-format
format:
	clang-format $(SRC_DIR)* $(CLANG_FORMAT_FLAGS) -i
	clang-format $(INCLUDE_DIR)* $(CLANG_FORMAT_FLAGS) -i

# Format check using clang-format
format-check:
	clang-format $(CLANG_FORMAT_FLAGS) --Werror --dry-run $(SRC_DIR)*
	clang-format $(CLANG_FORMAT_FLAGS) --Werror --dry-run $(INCLUDE_DIR)*

# Check if there are any "TODO" comments
todo:
	grep -r --color=auto -n TODO: $(SRC_DIR) $(INCLUDE_DIR)

# Check if there are any "NOTE" comments
note:
	grep -r --color=auto -n NOTE: $(SRC_DIR) $(INCLUDE_DIR)
