#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "moves.h"
#include "move.h"
#include "board.h"

Board board;
int turn; // 0: blue, 1: red

void init_board() {
    load_fen("9/3rp4/2rps4/1rps5/1ps3SP1/5SPR1/4SPR2/4PR3/9 b");
}

void load_fen(char *fen) {
    board = (Board){0, 0, 0, 0, 0};

    // assumes fen is valid
    int x = 0;
    int y = 8;
    while (*fen != '\0' && *fen != ' ') {
        if (islower(*fen)) {
            board.blue |= (BitBoard)1 << (9*y + x);
        } else if (isupper(*fen)) {
            board.red |= (BitBoard)1 << (9*y + x);
        } 

        if (*fen == '/') {
            y--;
            x = 0;
        } else if (isdigit(*fen)) {
            x += *fen - '0';
        } else if (tolower(*fen) == 'r') {
            board.r |= (BitBoard)1 << (9*y + x);
            x++;
        } else if (tolower(*fen) == 's') {
            board.s |= (BitBoard)1 << (9*y + x);
            x++;
        } else if (tolower(*fen) == 'p') {
            board.p |= (BitBoard)1 << (9*y + x);
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
    
    BitBoard temp = board.blue & (BitBoard)1 << move.from;
    board.blue ^= temp;
    board.blue |= (BitBoard)!!temp << move.to;
    board.red &= ~((BitBoard)!!temp << move.to);
    temp = board.red & (BitBoard)1 << move.from;
    board.red ^= temp;
    board.red |= (BitBoard)!!temp << move.to;
    board.blue &= ~((BitBoard)!!temp << move.to);
    temp = board.r & (BitBoard)1 << move.from;
    board.r ^= temp;
    board.r |= (BitBoard)!!temp << move.to;
    board.s &= ~((BitBoard)!!temp << move.to);
    temp = board.s & (BitBoard)1 << move.from;
    board.s ^= temp;
    board.s |= (BitBoard)!!temp << move.to;
    board.p &= ~((BitBoard)!!temp << move.to);
    temp = board.p & (BitBoard)1 << move.from;
    board.p ^= temp;
    board.p |= (BitBoard)!!temp << move.to;
    board.r &= ~((BitBoard)!!temp << move.to);
}

void unmake_move(Move move) {
    turn = ~turn;
    
    BitBoard tempbl = board.blue & (BitBoard)1 << move.to;
    board.blue ^= tempbl;
    board.blue |= (BitBoard)!!tempbl << move.from;
    
    BitBoard tempre = board.red & (BitBoard)1 << move.to;
    board.red ^= tempre;
    board.red |= (BitBoard)!!tempre << move.from;
    
    board.red |= tempbl & ((BitBoard)move.capture << move.to);
    board.blue |= tempre & ((BitBoard)move.capture << move.to);

    BitBoard tempr = board.r & (BitBoard)1 << move.to;
    board.r ^= tempr;
    board.r |= (BitBoard)!!tempr << move.from;

    BitBoard temps = board.s & (BitBoard)1 << move.to;
    board.s ^= temps;
    board.s |= (BitBoard)!!temps << move.from;

    BitBoard tempp = board.p & ((BitBoard)1 << move.to);
    board.p ^= tempp;
    board.p |= (BitBoard)!!tempp << move.from;
    
    board.r |= tempp & ((BitBoard)move.capture << move.to);
    board.s |= tempr & ((BitBoard)move.capture << move.to);
    board.p |= temps & ((BitBoard)move.capture << move.to);
}

void generate_moves(Move moves[80]) {
    int moves_ptr = 0;
    if (turn == 0) {
        BitBoard all_blue = board.blue;
        while (all_blue) {
            unsigned long long lower_pieces = all_blue & ULONG_LONG_MAX;
            int i = (lower_pieces != 0 ? __builtin_ctzll(lower_pieces) : (__builtin_ctzll(all_blue >> 64) + 64));
            all_blue ^= (BitBoard)1 << i;

            if ((board.p >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.blue | board.p | board.s);
                while (move_bb) {
                    unsigned long long lower_move = move_bb & ULONG_LONG_MAX;
                    int j = (lower_move != 0 ? __builtin_ctzll(lower_move) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.red >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.s >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.blue | board.r | board.s);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.red >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.r >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.blue | board.p | board.r);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.red >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            }
        }
    } else {
        BitBoard all_red = board.red;
        while (all_red) {
            unsigned long long lower_pieces = all_red & ULONG_LONG_MAX;
            int i = (lower_pieces != 0 ? __builtin_ctzll(lower_pieces) : (__builtin_ctzll(all_red >> 64) + 64));
            all_red ^= (BitBoard)1 << i;
        
            if ((board.p >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.red | board.p | board.s);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.blue >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.s >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.red | board.r | board.s);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.blue >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            } else if ((board.r >> i) & 1) {
                BitBoard move_bb = move_cache[i] & ~(board.red | board.p | board.r);
                while (move_bb) {
                    unsigned long long lower = move_bb & ULONG_LONG_MAX;
                    int j = (lower != 0 ? __builtin_ctzll(lower) : (__builtin_ctzll(move_bb >> 64) + 64));
                    Move move;
                    move.from = i;
                    move.to = j;
                    move.capture = (int)(board.blue >> j) & 1;
                    moves[moves_ptr++] = move;

                    move_bb ^= (BitBoard)1 << j;
                }
            }
        }
    }
    if (moves_ptr >= 80) {
        printf("%d", moves_ptr);
        output_board();

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
        if (get_bit(board.r, i) == 1) {
            out[i + i/9] = 'r' - 32 * get_bit(board.red, i);
        } else if (get_bit(board.s, i) == 1) {
            out[i + i/9] = 's' - 32 * get_bit(board.red, i);
        } else if (get_bit(board.p, i) == 1) {
            out[i + i/9] = 'p' - 32 * get_bit(board.red, i);
        } else {
            out[i + i/9] = '.';
        }
    }
    for (int i = 1; i < 9; i++) {
        out[i*10-1] = '\n';
    }
    printf("%s \n", out);
}