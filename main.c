#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "move.h"
#include "board.h"

int counts[6] = {0};

void test_move_gen(int depth) {
    counts[depth] += 1;
    
    if (depth == 0) { return; }
    
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
    output_board();
    test_move_gen(5);
    for(int i = 0; i < 6; i++) {
        printf("%d, %d\n", i, counts[i]);
    }
    
}