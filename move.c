#include <stdio.h>
#include <string.h>

#include "move.h"

// moves should be formatted as (([a-i][1-9])-\2)|(([a-i][1-9])x\4([br][rsp])?) to comply with https://rps.henhen1227.com/account/bots/connect while also allowing for capture field
//                                from-to       or  fromxto      or fromxtocapture
// won't validate
Move process_move(char* movestr) {
    int length = strlen(movestr)/sizeof(movestr[0]);
    printf("move len: %d\n", length);
    if (length != 6 && length != 8) {
        printf("move is invalid\n");
    }
    
    Move move;
    
    // from
    int y = movestr[0] - 'a';
    int x = movestr[1] - '0' - 1;
    move.from = y*9 + x;

    // to
    y = movestr[3] - 'a';
    x = movestr[4] - '0' - 1;
    move.to = y*9 + x;

    move.capture = 0;
    if (length == 8) {
        if (movestr[5] == 'r') {
            move.capture += 3;
        }
        switch (movestr[6]) {
            case 'r':
                move.capture += 1;
                break;
            case 's':
                move.capture += 2;
                break;
            case 'p':
                move.capture += 3;
                break;
        }
    }
    return move;
}