#include <stdio.h>

#include "board.h"

Board board;

void init_board() {
    BitBoard blueR = ((BitBoard)4 << 64) + 72127962782105600;
    BitBoard blueS = ((BitBoard)0 << 64) + 288512126006329344;
    BitBoard blueP = ((BitBoard)8 << 64) + 144256063003164672;
    BitBoard redR = 17196662784;
    BitBoard redS = 4402345672704;
    BitBoard redP = 8804691353600;
    board = (Board){blueR, blueS, blueP, redR, redS, redP};
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