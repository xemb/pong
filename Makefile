# Compiler
CC       := clang
CFLAGS   := -Wall -Wextra -std=c99 -O2
# Header path: assumes raylib.h is in ~/.local/include
INCLUDES := -I$(HOME)/.local/include

# Sources and targets
SRC_DIR   := src
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/main

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Static library path and required system libraries
RAYLIB_STATIC := $(HOME)/.local/lib/libraylib.a
LDLIBS       := $(RAYLIB_STATIC) -lGL -lm -lpthread -ldl -lrt -lX11

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# Compile each .c file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run
run: all
	./$(TARGET)

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean