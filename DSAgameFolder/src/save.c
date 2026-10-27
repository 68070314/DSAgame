#include <stdio.h>
#include "save.h"

SaveData save_load(const char *path, int num_levels) {
    SaveData d = { 0, 0 };

    FILE *f = fopen(path, "r");
    if (!f) return d;                       // first run: no save yet

    int unlocked = 0;
    int last = -1;
    int got = fscanf(f, "%d %d", &unlocked, &last);
    fclose(f);

    if (got < 1) unlocked = 0;              // empty or garbage file
    if (got < 2) last = unlocked;           // old one-number save file

    if (unlocked < 0) unlocked = 0;
    if (unlocked >= num_levels) unlocked = num_levels - 1;
    if (last < 0) last = 0;
    if (last > unlocked) last = unlocked;   // can't have played a locked level

    d.unlocked = unlocked;
    d.last = last;
    return d;
}

bool save_write(const char *path, const SaveData *data) {
    FILE *f = fopen(path, "w");
    if (!f) return false;

    bool ok = fprintf(f, "%d %d\n", data->unlocked, data->last) > 0;
    if (fclose(f) != 0) ok = false;         // fclose flushes the data to disk
    return ok;
}
