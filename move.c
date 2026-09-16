#include <stdio.h>
#include <string.h>

#include "move.h"

// moves should be formatted as ([a-i][1-9])[-x]\2) to comply with https://rps.henhen1227.com/account/bots/connect
//                                from-to       or  fromxto
// won't validate
Move process_move(char* movestr) {
    int length = strlen(movestr)/sizeof(movestr[0]);
    printf("move len: %d\n", length);
    if (length != 6) {
        printf("move is invalid\n");
        return (Move){0, 0, 0};
    }
    
    Move move;
    
    // from
    int x = (movestr[0] - 'a');
    int y = 8 - (movestr[1] - '1');
    move.from = y*9 + x;

    // to
    x = (movestr[3] - 'a');
    y = 8 - (movestr[4] - '1');
    move.to = y*9 + x;

    move.capture = 0;
    if (movestr[2] == 'x') {
        move.capture = 1;
    }
    return move;
}