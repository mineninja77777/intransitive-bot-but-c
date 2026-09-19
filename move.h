#ifndef MOVE_H
#define MOVE_H

typedef struct {
    int from;
    int to;
    int capture; // to help with unmoving, as pieces can only capture one type of piece it is 0 or 1
} Move;

Move process_move(char* movestr);
void output_move(Move move);

#endif