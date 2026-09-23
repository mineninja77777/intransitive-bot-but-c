#include <stdlib.h>
#include <stdio.h>

#include "precomputed.h"
#include "move.h"
#include "board.h"
#include "engine.h"

#define popcnt(bb) (__builtin_popcountll(bb >> 64) + __builtin_popcountll(bb & ULLONG_MAX))

Move best_move() {
    
    int turn = get_turn();

    int best_eval = __INT16_MAX__ * (turn);
    Move best_move = (Move){0,0,0};

    Move moves[80] = {(Move){0, 0, 0}};
    generate_moves(moves);
    Move *curr = &(moves[0]);
    // output_move(*curr);
    while (!(curr->from == 0 && curr->to == 0 && curr->capture == 0)) {
        make_move(*curr);
        int val = minimax(3);
        // printf("%d\n", turn);
        unmake_move(*curr);
        if ((val >= best_eval && turn == -1) || (val <= best_eval && turn == 1)) {
            best_eval = val;
            best_move = *curr;
        }

        curr += 1;
    }
    output_move(best_move);
    return best_move;
}

int minimax(int depth) {
    if (game_state() != 0) {
        return ((2 - game_state()) * __INT16_MAX__);
    }
    if (depth == 0) {
        return eval();
    }

    int turn = get_turn();

    int best_eval = __INT16_MAX__ * (turn);

    Move moves[80] = {(Move){0, 0, 0}};
    generate_moves(moves);
    Move *curr = &(moves[0]);
    while (!(curr->from == 0 && curr->to == 0 && curr->capture == 0)) {
        make_move(*curr);
        int val = minimax(depth-1);
        // printf("best_eval: %d eval: %d, turn: %d\n", best_eval, val, turn);
        unmake_move(*curr);
        if ((val > best_eval && turn == -1) || (val < best_eval && turn == 1)) {
            best_eval = val;
        }

        curr += 1;
    }

    return best_eval;
}

int eval() {
    Board board = get_board();

    BitBoard br = board.blue & board.r;
    BitBoard bs = board.blue & board.s;
    BitBoard bp = board.blue & board.p;
    BitBoard rr = board.red & board.r;
    BitBoard rs = board.red & board.s;
    BitBoard rp = board.red & board.p;

    int br_count = popcnt(br);
    int bs_count = popcnt(bs);
    int bp_count = popcnt(bp);
    int rr_count = popcnt(rr);
    int rs_count = popcnt(rs);
    int rp_count = popcnt(rp);
    
    int matchup_adv = __max(br_count - rs_count, 
                      __max(bs_count - rp_count, 
                      bp_count - rr_count)) 
                    - __max(rr_count - bs_count, 
                      __max(rs_count - bp_count, 
                      rp_count - br_count));
    
    int piece_square_bonus = 0;
    BitBoard all_blue = board.blue;
    while (all_blue) {
        unsigned long long lower_pieces = all_blue & ULONG_LONG_MAX;
        int i = (lower_pieces != 0 ? __builtin_ctzll(lower_pieces) : (__builtin_ctzll(all_blue >> 64) + 64));
        all_blue ^= (BitBoard)1 << i;

        piece_square_bonus += blue_piece_square_table[i];
    }

    BitBoard all_red = board.red;
    while (all_red) {
        unsigned long long lower_pieces = all_red & ULONG_LONG_MAX;
        int i = (lower_pieces != 0 ? __builtin_ctzll(lower_pieces) : (__builtin_ctzll(all_red >> 64) + 64));
        all_red ^= (BitBoard)1 << i;

        piece_square_bonus -= red_piece_square_table[i];
    }

    return (popcnt(board.blue) - popcnt(board.red)) * 100
            + matchup_adv * 200
            + piece_square_bonus;
}