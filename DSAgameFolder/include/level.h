#ifndef LEVEL_H
#define LEVEL_H
#include <stdbool.h>

#define MAX_W 20
#define MAX_H 20

typedef struct {
    char grid[MAX_H][MAX_W];  // '#' wall, ' ' floor, '.' goal, '$' box, '@' player, '*' box on goal, '+' player on goal
    int width, height;
    int player_x, player_y;
    int moves;
} Level;

bool level_load(Level *lv, const char *path);

#endif