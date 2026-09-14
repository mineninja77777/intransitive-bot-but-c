#ifndef BOARD_H
#define BOARD_H

typedef unsigned __int128 BitBoard;
typedef struct {
    BitBoard blueR;
    BitBoard blueS;
    BitBoard blueP;
    BitBoard redR;
    BitBoard redS;
    BitBoard redP;
} Board;

void init_board();
int get_bit(BitBoard board, int bit);
void set_bit(BitBoard *board, int bit, int value);
void output_bitboard(BitBoard board);
void output_board();

#endif