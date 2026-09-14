
#include "move.h"
#include "board.h"

#include <stdio.h>

int main() {

    // while (1) {
    //     char line[20];
    //     fgets(line, sizeof(line), stdin);
    //     Move move = process_move(line);
    //     printf("%d, %d, %d\n", move.from, move.to, move.capture);
    // }
    init_board();
    output_board();

    // init_board();
    // output_board();
    // BitBoard bb = {0,0};
    // int address;
    // for (;;) {
    //     output_bitboard(bb);
    //     printf("board: %llu %llu\n", bb.high, bb.low);
    //     printf("input number: ");

    //     char line[20];
    //     fgets(line, sizeof(line), stdin);
    //     sscanf(line, "%d", &address);
    //     if (address > 81) {
    //         return 0;
    //     }
        
    //     set_bit(&bb, address, 1);
    // }
}