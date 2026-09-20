#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "move.h"
#include "board.h"

int count = 0;

void test_move_gen(int depth) {
    if (depth == 0) { 
        count++; 
        return; 
    }
    
    Move moves[80] = {(Move){0, 0, 0}};
    generate_moves(moves);
    Move *curr = &(moves[0]);
    while (!(curr->from == 0 && curr->to == 0 && curr->capture == 0)) {
        make_move(*curr);
        test_move_gen(depth-1);
        unmake_move(*curr);
        curr += 1;
    }
}

void benchmark() {
    for(int i = 1; i < 6; i++) {
        clock_t start, stop;
        count = 0;
        start = clock();
        test_move_gen(i);
        stop = clock();
        printf("%d, %dms, %d\n", i, (int)((double)(stop-start) / CLOCKS_PER_SEC * 1000), count);
    }
}

int main() {
    // init_board();
    load_fen("9/9/9/9/9/9/s8/rRp6/RPS6 r");
    benchmark();

    // Move move;
    // while (1) {
    //     char move_s[20];
    //     fgets(move_s, 19, stdin);
    //     if (strncmp(move_s, "undo", 4) == 0) {
    //         unmake_move(move);
    //     } else {
    //         move = process_move(move_s);
    //         make_move(move);
    //     }
    //     output_board();
    // }

    // Move moves[80] = {(Move){0, 0, 0}};
    // generate_moves(moves);
    // Move *curr = &(moves[0]);
    // while (!(curr->from == 0 && curr->to == 0 && curr->capture == 0)) {
    //     output_move(*curr);
    //     curr += 1;
    // }
}