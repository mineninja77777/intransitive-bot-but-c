#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "moves.h"
#include "move.h"
#include "board.h"

Board board;
int turn; // 0: blue, 1: red

void init_board() {
    BitBoard blueR = ((BitBoard)4 << 64) + 72127962782105600;
    BitBoard blueS = 288512126006329344;
    BitBoard blueP = ((BitBoard)8 << 64) + 144256063003164672;
    BitBoard redR = 17196662784;
    BitBoard redS = 4402345672704;
    BitBoard redP = 8804691353600;
    board = (Board){blueR, blueS, blueP, redR, redS, redP};
    turn = 0;
}

void load_fen(char *fen) {
    board = (Board){0, 0, 0, 0, 0, 0};

    // assumes fen is valid
    int x = 0;
    int y = 8;
    while (*fen != '\0' && *fen != ' ') {
        if (*fen == '/') {
            y--;
            x = 0;
        } else if (isdigit(*fen)) {
            x += *fen - '0';
        } else if (*fen == 'r') {
            board.blueR |= (BitBoard)1 << (9*y + x);
            x++;
        } else if (*fen == 's') {
            board.blueS |= (BitBoard)1 << (9*y + x);
            x++;
        } else if (*fen == 'p') {
            board.blueP |= (BitBoard)1 << (9*y + x);
            x++;
        } else if (*fen == 'R') {
            board.redR |= (BitBoard)1 << (9*y + x);
            x++;
        } else if (*fen == 'S') {
            board.redS |= (BitBoard)1 << (9*y + x);
            x++;
        } else if (*fen == 'P') {
            board.redP |= (BitBoard)1 << (9*y + x);
            x++;
        } else {
            fprintf(stderr, "mysterious....\n");
        }
        fen++;
    }
    fen++;
    if (*fen == 'b') {
        turn = 0;
    } else if (*fen == 'r') {
        turn = 1;
    }
}

int get_bit(BitBoard board, int bit) {
    return (board >> bit) & 1;
}

void set_bit(BitBoard *board, int bit, int value) {
    if (get_bit(*board, bit) == value) {
        return;
    }
    (*board) ^= (BitBoard)1 << bit;
}

// assumes move is valid
void make_move(Move move) {
    turn = ~turn;
    if (get_bit(board.blueP, move.from)) {
        set_bit(&board.blueP, move.from, 0);
        set_bit(&board.blueP, move.to, 1);
        set_bit(&board.redR, move.to, 0); // could only be taking enemy rock
    } else if (get_bit(board.blueS, move.from)) {
        set_bit(&board.blueS, move.from, 0);
        set_bit(&board.blueS, move.to, 1);
        set_bit(&board.redP, move.to, 0);
    } else if (get_bit(board.blueR, move.from)) {
        set_bit(&board.blueR, move.from, 0);
        set_bit(&board.blueR, move.to, 1);
        set_bit(&board.redS, move.to, 0);
    } else if (get_bit(board.redP, move.from)) {
        set_bit(&board.redP, move.from, 0);
        set_bit(&board.redP, move.to, 1);
        set_bit(&board.blueR, move.to, 0);
    } else if (get_bit(board.redS, move.from)) {
        set_bit(&board.redS, move.from, 0);
        set_bit(&board.redS, move.to, 1);
        set_bit(&board.blueP, move.to, 0);
    } else if (get_bit(board.redR, move.from)) {
        set_bit(&board.redR, move.from, 0);
        set_bit(&board.redR, move.to, 1);
        set_bit(&board.blueS, move.to, 0);
    } else {
        printf("Invalid movea %d %d %d\n", move.from, move.to, move.capture);
        
        output_board();

    } 
}

void unmake_move(Move move) {
    turn = ~turn;
    if (get_bit(board.blueP, move.to)) {
        set_bit(&board.blueP, move.from, 1);
        set_bit(&board.blueP, move.to, 0);
        set_bit(&board.redR, move.to, move.capture); // could only be taking enemy rock
    } else if (get_bit(board.blueS, move.to)) {
        set_bit(&board.blueS, move.from, 1);
        set_bit(&board.blueS, move.to, 0);
        set_bit(&board.redP, move.to, move.capture);
    } else if (get_bit(board.blueR, move.to)) {
        set_bit(&board.blueR, move.from, 1);
        set_bit(&board.blueR, move.to, 0);
        set_bit(&board.redS, move.to, move.capture);
    } else if (get_bit(board.redP, move.to)) {
        set_bit(&board.redP, move.from, 1);
        set_bit(&board.redP, move.to, 0);
        set_bit(&board.blueR, move.to, move.capture);
    } else if (get_bit(board.redS, move.to)) {
        set_bit(&board.redS, move.from, 1);
        set_bit(&board.redS, move.to, 0);
        set_bit(&board.blueP, move.to, move.capture);
    } else if (get_bit(board.redR, move.to)) {
        set_bit(&board.redR, move.from, 1);
        set_bit(&board.redR, move.to, 0);
        set_bit(&board.blueS, move.to, move.capture);
    } else {
        printf("Invalid moveb %d %d %d\n", move.from, move.to, move.capture);
        
        output_board();
    } 
}

void generate_moves(Move moves[80]) {
    int moves_ptr = 0;
    if (turn == 0) {
        for (int i = 0; i < 81; i++) {
            if ((board.blueP >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.blueP | board.blueR | board.blueS | board.redP | board.redS);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.redR >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.blueS >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.blueP | board.blueR | board.blueS | board.redR | board.redS);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.redP >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.blueR >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.blueP | board.blueR | board.blueS | board.redP | board.redR);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.redS >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            }
        }
    } else {
        for (int i = 0; i < 81; i++) { 
            if ((board.redP >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.redP | board.redR | board.redS | board.blueP | board.blueS);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.blueR >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.redS >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.redP | board.redR | board.redS | board.blueR | board.blueS);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.blueP >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.redR >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.redP | board.redR | board.redS | board.blueP | board.blueR);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.blueS >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            }
        }
    }
}

void output_bitboard(BitBoard board) {
    // complications due to wanting line breaks

    char out[90] = {};
    for (int i = 0; i < 81; i++) {
        if (get_bit(board, i) == 0) {
            out[i + i/9] = '0';
        } else {
            out[i + i/9] = '1';
        }
    }
    for (int i = 1; i < 9; i++) {
        out[i*10-1] = '\n';
    }
    printf("%s \n", out);
}

void output_board() {
    // complications due to wanting line breaks

    char out[90] = {};
    for (int i = 0; i < 81; i++) {
        if (get_bit(board.blueR, i) == 1) {
            out[i + i/9] = 'r';
        } else if (get_bit(board.blueS, i) == 1) {
            out[i + i/9] = 's';
        } else if (get_bit(board.blueP, i) == 1) {
            out[i + i/9] = 'p';
        } else if (get_bit(board.redR, i) == 1) {
            out[i + i/9] = 'R';
        } else if (get_bit(board.redS, i) == 1) {
            out[i + i/9] = 'S';
        } else if (get_bit(board.redP, i) == 1) {
            out[i + i/9] = 'P';
        } else {
            out[i + i/9] = '.';
        }
    }
    for (int i = 1; i < 9; i++) {
        out[i*10-1] = '\n';
    }
    printf("%s \n", out);
}