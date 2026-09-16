#include <stdio.h>
#include <stdlib.h>

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
        printf("Invalid move\n");
    } 
}

void unmake_move(Move move) {
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
        printf("Invalid move\n");
    } 
}

void generate_moves(Move moves[80]) {
    // TODO: yeah this
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