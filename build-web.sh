#!/bin/sh
set -eu

if ! command -v emcc >/dev/null 2>&1; then
    echo "Error: emcc was not found. Install and activate Emscripten first." >&2
    exit 1
fi

emcc tictactoe.c \
  -std=c11 -O2 \
  -s WASM=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME=createTicTacToeModule \
  -s EXPORTED_FUNCTIONS='["_game_reset","_game_move","_game_get_cell","_game_get_current_player","_game_get_status"]' \
  -s EXPORTED_RUNTIME_METHODS='[]' \
  -s ENVIRONMENT=web \
  -o tictactoe.js

echo "Build complete: tictactoe.js and tictactoe.wasm"
echo "Run: python3 -m http.server 8000"
echo "Open: http://localhost:8000/"
