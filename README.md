# [Tic-Tac-Toe](https://requiemdoomed.github.io/Tic-Tac-Toe/)

**Author: Swayam Shakya**

A terminal Tic-Tac-Toe game and a responsive browser UI powered by the same C game engine.

## Files

- `tictactoe.c` — C engine plus native terminal `main()`
- `index.html` — responsive interface that calls the compiled C functions
- `build-web.sh` — builds the C engine into WebAssembly with Emscripten
- `Makefile` — native Unix build and clean targets

## Why WebAssembly?

A web page cannot execute a `.c` source file directly. Emscripten compiles the C source to `tictactoe.js` plus `tictactoe.wasm`. The HTML interface calls exported C functions such as `game_move()` and `game_get_status()`. The browser's JavaScript handles presentation only; game state and move validation live in C.

## Native build on OpenBSD/Linux/macOS

```sh
cc -std=c11 -Wall -Wextra -Wpedantic tictactoe.c -o tictactoe
./tictactoe
```

Or use:

```sh
make
./tictactoe
```

## Browser build

Install Emscripten and make sure `emcc` is on your PATH. Then run:

```sh
chmod +x build-web.sh
./build-web.sh
```

This generates `tictactoe.js` and `tictactoe.wasm` in the project directory.

The build command exports the C API and names the JavaScript factory `createTicTacToeModule`, which `index.html` expects.

## Serve the page locally

Browsers often block WebAssembly loading from `file://`, so start a local HTTP server from this folder:

```sh
python3 -m http.server 8000
```

Open `http://localhost:8000/` in your browser.

## Controls and features

- Two-player local game
- Input validation in C
- Win and draw detection in C
- Round and score tracking in the UI
- Highlighted winning cells
- Move history
- Responsive layout and keyboard-accessible buttons
- Reduced-motion preference support

