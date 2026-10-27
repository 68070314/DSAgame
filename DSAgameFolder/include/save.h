#ifndef SAVE_H
#define SAVE_H
#include <stdbool.h>

typedef struct {
    int unlocked;   // highest level index (0-based) the player may play
    int last;       // level index (0-based) the player opened most recently
} SaveData;

// Reads save.txt. A missing or unreadable file gives {0, 0}.
// Old save files that hold only the unlocked level still load fine.
// Values are clamped to 0..num_levels-1, and last is never above unlocked.
SaveData save_load(const char *path, int num_levels);

// Writes the save data to save.txt. Returns false on failure.
bool save_write(const char *path, const SaveData *data);

#endif
