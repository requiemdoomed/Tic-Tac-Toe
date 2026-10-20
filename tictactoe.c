/*
 * Tic-Tac-Toe: C Game Engine
 * Author: Swayam Shakya
 *
 * Shared game engine for:
 *   1. Native Unix/OpenBSD terminal build
 *   2. Browser build compiled to WebAssembly with Emscripten
 *
 * The browser calls these exported C functions through tictactoe.js.
 */

#include <stdio.h>
#include <stdlib.h>

#define BOARD_CELLS 9

static char board[BOARD_CELLS];
static char current_player = 'X';
static int game_over = 0;
static char winner = '\0';

static const int wins[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
};

static void reset_state(void)
{
    int i;
    for (i = 0; i < BOARD_CELLS; ++i) {
        board[i] = ' ';
    }
    current_player = 'X';
    game_over = 0;
    winner = '\0';
}

static int has_winner(void)
{
    int i;
    for (i = 0; i < 8; ++i) {
        int a = wins[i][0];
        int b = wins[i][1];
        int c = wins[i][2];
        if (board[a] != ' ' && board[a] == board[b] && board[b] == board[c]) {
            return 1;
        }
    }
    return 0;
}

static int board_is_full(void)
{
    int i;
    for (i = 0; i < BOARD_CELLS; ++i) {
        if (board[i] == ' ') return 0;
    }
    return 1;
}

static void update_game_state(void)
{
    if (has_winner()) {
        game_over = 1;
        winner = current_player;
    } else if (board_is_full()) {
        game_over = 1;
        winner = 'D'; /* Draw */
    } else {
        current_player = (current_player == 'X') ? 'O' : 'X';
    }
}

/* Exported API: used by JavaScript after compiling with Emscripten. */
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define API EMSCRIPTEN_KEEPALIVE
#else
#define API
#endif

API void game_reset(void)
{
    reset_state();
}

API int game_move(int position)
{
    if (position < 0 || position >= BOARD_CELLS || game_over || board[position] != ' ') {
        return 0;
    }

    board[position] = current_player;
    update_game_state();
    return 1;
}

API int game_get_cell(int position)
{
    if (position < 0 || position >= BOARD_CELLS) return -1;
    if (board[position] == 'X') return 1;
    if (board[position] == 'O') return 2;
    return 0;
}

API int game_get_current_player(void)
{
    return (current_player == 'X') ? 1 : 2;
}

/* 0 = ongoing, 1 = X wins, 2 = O wins, 3 = draw */
API int game_get_status(void)
{
    if (!game_over) return 0;
    if (winner == 'X') return 1;
    if (winner == 'O') return 2;
    return 3;
}

#ifndef __EMSCRIPTEN__
static void print_board(void)
{
    int i;
    puts("");
    for (i = 0; i < BOARD_CELLS; ++i) {
        char shown = board[i] == ' ' ? (char)('1' + i) : board[i];
        printf(" %c ", shown);
        if (i % 3 != 2) printf("|");
        else if (i != BOARD_CELLS - 1) puts("\n---+---+---");
    }
    puts("\n");
}

int main(void)
{
    char line[128];
    char *end;
    long move;

    game_reset();
    puts("==================================");
    puts("       TIC-TAC-TOE IN C");
    puts("==================================");
    puts("Two players: X and O. Enter 1-9.");

    while (game_get_status() == 0) {
        print_board();
        printf("Player %c, choose a cell: ", game_get_current_player() == 1 ? 'X' : 'O');

        if (!fgets(line, sizeof line, stdin)) {
            puts("\nInput ended. Exiting.");
            return EXIT_SUCCESS;
        }

        move = strtol(line, &end, 10);
        while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') ++end;
        if (end == line || *end != '\0' || move < 1 || move > 9) {
            puts("Please enter a whole number from 1 to 9.");
            continue;
        }

        if (!game_move((int)move - 1)) {
            puts("That cell is already occupied. Choose an empty cell.");
        }
    }

    print_board();
    if (game_get_status() == 1) puts("Player X wins!");
    else if (game_get_status() == 2) puts("Player O wins!");
    else puts("It's a draw!");

    return EXIT_SUCCESS;
}

#endif /* !__EMSCRIPTEN__ */
