#ifndef LEVEL_H
#define LEVEL_H
#include <stdbool.h>
#include <stddef.h>

#define MAX_W 20
#define MAX_H 20
#define MAX_LEVELS 25   // the level tables have room for 5 rows of 5 buttons

typedef struct {
    char grid[MAX_H][MAX_W];  // '#' wall, ' ' floor, '.' goal, '$' box, '@' player, '*' box on goal, '+' player on goal
    int width, height;
    int player_x, player_y;
    int moves;
} Level;

bool level_load(Level *lv, const char *path);

// Writes the level as a text file (one line per row). Returns false on failure.
bool level_save(const Level *lv, const char *path);

// Builds the file name of a level: index 0 gives "levels/level01.txt".
void level_path(int index, char *out, size_t size);

// Counts the level files: level01.txt, level02.txt, ... It stops at the first
// missing number, and at MAX_LEVELS.
int level_count(void);

#endif
