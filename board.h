#ifndef BOARD_H
#define BOARD_H

typedef unsigned __int128 BitBoard;
typedef struct {
    BitBoard blue;
    BitBoard red;
    BitBoard r;
    BitBoard s;
    BitBoard p;
} Board;

void init_board();
void load_fen(char *fen);
int get_bit(BitBoard board, int bit);
void set_bit(BitBoard *board, int bit, int value);
void make_move(Move move);
void unmake_move(Move move);
void generate_moves(Move moves[80]);
void output_bitboard(BitBoard board);
void output_board();

#endif