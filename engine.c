#include <stdlib.h>
#include <stdio.h>

#include "precomputed.h"
#include "move.h"
#include "board.h"
#include "engine.h"

#define popcnt(bb) (__builtin_popcountll(bb >> 64) + __builtin_popcountll(bb & ULLONG_MAX))
#define min __min
#define max __max

Move best_move() {
    
    int turn = get_turn();

    int best_eval = (turn == -1 ? INT_MIN : INT_MAX);
    Move best_move = (Move){0,0,0};

    Move moves[80] = {(Move){0, 0, 0}};
    generate_moves(moves);
    Move *curr = &(moves[0]);
    while (!(curr->from == 0 && curr->to == 0 && curr->capture == 0)) {
        make_move(*curr);
        int val = minimax(4, 4, INT_MIN, INT_MAX);
        unmake_move(*curr);
        if ((val >= best_eval && turn == -1) || (val <= best_eval && turn == 1)) {
            best_eval = val;
            best_move = *curr;
        }

        curr += 1;
    }
    return best_move;
}

// alpha = INT_MIN, beta = INT_MAX
int minimax(int depth, int max_depth, int alpha, int beta) {
    if (game_state() != 0) {
        return ((game_state() - 2) == -1 ? (INT_MAX - (max_depth - depth)) : (INT_MIN + (max_depth - depth)));
    }
    if (depth == 0) {
        return eval();
    }

    int turn = get_turn();

    int eval = (turn == -1 ? INT_MIN : INT_MAX);

    Move moves[80] = {(Move){0, 0, 0}};
    generate_moves(moves);
    Move *curr = &(moves[0]);
    while (!(curr->from == 0 && curr->to == 0 && curr->capture == 0)) {
        if (turn == -1) {
            make_move(*curr);
            eval = max(eval, minimax(depth-1, max_depth, alpha, beta));
            unmake_move(*curr);

            if (eval >= beta) {
                break;
            }
            alpha = max(alpha, eval);
        } else {
            make_move(*curr);
            eval = min(eval, minimax(depth-1, max_depth, alpha, beta));
            unmake_move(*curr);

            if (eval <= alpha) {
                break;
            }
            beta = min(beta, eval);
        }
        curr += 1;
    }

    return eval;
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