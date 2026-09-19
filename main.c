#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

int main() {
    init_board();
    

    for(int i = 1; i < 10; i++) {
        clock_t start, stop;
        count = 0;
        start = clock();
        test_move_gen(i);
        stop = clock();
        printf("%d, %dms, %d\n", i, (int)((double)(stop-start) / CLOCKS_PER_SEC * 1000), count);
    }
    // load_fen("9/4p2p1/2p6/4p4/3p5/9/9/1p7/9 b");
    // output_board();
    // Move moves[80] = {(Move){0, 0, 0}};
    // generate_moves(moves);
    // Move *curr = &(moves[0]);
    // while (!(curr->from == 0 && curr->to == 0 && curr->capture == 0)) {
    //     output_move(*curr);
    //     curr += 1;
    // }
}