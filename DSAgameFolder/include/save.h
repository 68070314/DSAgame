#ifndef SAVE_H
#define SAVE_H
#include <stdbool.h>

// Reads the highest unlocked level index (0-based) from save.txt.
// Returns 0 if there is no save file or it is unreadable.
int save_load(int num_levels);

// Writes the highest unlocked level index to save.txt. Returns false on failure.
bool save_write(int unlocked);

#endif
