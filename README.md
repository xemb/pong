# Pong

Pong is a simple ping-pong game built with Raylib.

## Dependencies

- C Compiler: `clang` (or `gcc`, adjust `CC` in the Makefile)
- Raylib
- System libraries (linked dynamically): OpenGL, X11, pthread
- Build tool: `make`

## How to build

Modify Makefile, where sets your header and library path.
Example:

```Makefile
# Example: my header at ~/.local/include
# lib at ~/.local/lib

INCLUDES := -I$(HOME)/.local/include
RAYLIB_STATIC := $(HOME)/.local/lib/libraylib.a
```

Then, run `make` or `make run` to build and run.
By default, the products are in build/ folder.

## How to play

Press `A`/`D` to control the tray,
press `W`/`S` to speed up or down.