#ifndef GAME_H
#define GAME_H
#include "level.h"

// Returns true if the move changed the level, false if it was blocked.
bool game_move(Level *lv, int dx, int dy);
bool game_is_won(const Level *lv);

#endif
