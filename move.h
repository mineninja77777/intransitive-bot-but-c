#ifndef MOVE_H
#define MOVE_H

typedef struct {
    int from;
    int to;
    int capture; // to help with unmoving, if the move was a capture then it would say the piece type (just counting following order of the items in struct Board), otherwise zero
} Move;

Move process_move(char* movestr);

#endif