# Pong

Pong is a simple ping-pong game built with Raylib.

## Dependencies

- C Compiler: `gcc` (MinGW-w64 on Windows) or `clang`, adjustable via `CC`
- Raylib headers and libraries, kept in `include/` and `lib/`
- Build tool: `make` (Linux), `mingw32-make` (Windows)

The Makefile detects the platform itself:

| Platform | Compiler | Raylib library | Extra libraries |
| -------- | -------- | -------------- | --------------- |
| Linux    | `gcc`    | `lib/libraylib.a` | `-lGL -lm -lpthread -ldl -lrt -lX11` |
| Windows  | `gcc` (MinGW-w64) | `lib/libraylib.a` | `-lopengl32 -lgdi32 -lwinmm` |

On Windows the executable is linked statically (`-static`), so
`build/main.exe` runs without `raylib.dll` or any MinGW runtime DLL beside it.

## How to build

Headers are expected in `include/` and libraries in `lib/`. To use a Raylib
installed somewhere else, change the paths at the top of the Makefile:

```Makefile
# Default: the copies bundled with this repository
INCLUDES := -Iinclude
RAYLIB_LIBS := lib/libraylib.a -lopengl32 -lgdi32 -lwinmm
```

### Windows (MinGW-w64)

Make sure `gcc` and `mingw32-make` are on `PATH`, then from the project folder:

```powershell
mingw32-make          # build -> build\main.exe
mingw32-make run      # build and start the game
mingw32-make clean    # remove build\
```

If MinGW is not on `PATH` (for example `C:\Users\me\Apps\mingw64\bin`),
add it for the current shell:

```powershell
$env:PATH = "C:\Users\me\Apps\mingw64\bin;$env:PATH"
```

or point the Makefile at a specific compiler:
`mingw32-make CC=C:/Users/me/Apps/mingw64/bin/gcc.exe`.

### Linux

```sh
make        # build -> build/main
make run
make clean
```

By default, the products are in the `build/` folder.

## How to play

Press `A`/`D` to control the tray,
press `W`/`S` to speed up or down.
