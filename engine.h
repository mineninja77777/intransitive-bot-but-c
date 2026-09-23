#ifndef ENGINE_H
#define ENGINE_H

#include "move.h"

Move best_move();
int minimax(int depth);
int eval();

#endif