# ---------------------------------------------------------------------------
# Pong
#
# Works with:
#   * Windows : MinGW-w64 gcc + mingw32-make (raylib win64_mingw-w64 build)
#   * Linux   : gcc/clang + make (raylib built for X11)
#
# raylib's headers and libraries are expected in ./include and ./lib, so the
# checkout builds as-is without editing any absolute paths.
# ---------------------------------------------------------------------------

# Directories
SRC_DIR   := src
BUILD_DIR := build
LIB_DIR   := lib

# ---------------------------------------------------------------------------
# Platform detection
# ---------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)

  EXE := .exe

  # Recipes below use cmd.exe syntax; pin it so they keep working even when a
  # POSIX shell (Git Bash / MSYS2) happens to be on PATH.
  SHELL := cmd.exe

  MKDIR = if not exist "$(1)" mkdir "$(1)"
  RMDIR = if exist "$(1)" rmdir /S /Q "$(1)"

  # raylib's MinGW-w64 static library plus the Windows libraries it needs.
  RAYLIB_LIBS := $(LIB_DIR)/libraylib.a -lopengl32 -lgdi32 -lwinmm

  # Produce a self-contained .exe: no libgcc/winpthread DLLs to ship.
  PLATFORM_LDFLAGS := -static -static-libgcc

  # cmd.exe reads "build/main.exe" as "build" + switch "/main.exe",
  # so the run target needs a backslash path here.
  RUN_CMD := $(BUILD_DIR)\main$(EXE)

else

  EXE :=

  MKDIR = mkdir -p "$(1)"
  RMDIR = rm -rf "$(1)"

  RAYLIB_LIBS := $(LIB_DIR)/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
  PLATFORM_LDFLAGS :=
  RUN_CMD := ./$(BUILD_DIR)/main$(EXE)

endif

# ---------------------------------------------------------------------------
# Compiler and flags
# ---------------------------------------------------------------------------
# Override on the command line if needed, e.g. `make CC=clang`
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c99 -O2
INCLUDES := -Iinclude

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
TARGET := $(BUILD_DIR)/main$(EXE)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(PLATFORM_LDFLAGS) $(RAYLIB_LIBS)

# Compile each .c file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create the build directory (order-only prerequisite)
$(BUILD_DIR):
	@$(call MKDIR,$(BUILD_DIR))

# Run
run: $(TARGET)
	$(RUN_CMD)

# Clean
clean:
	@$(call RMDIR,$(BUILD_DIR))

.PHONY: all run clean
