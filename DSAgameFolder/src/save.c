#include <stdio.h>
#include "save.h"

int save_load(const char *path, int num_levels) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;                       // first run: no save yet

    int v = 0;
    if (fscanf(f, "%d", &v) != 1) v = 0;    // empty or garbage file
    fclose(f);

    if (v < 0) v = 0;
    if (v >= num_levels) v = num_levels - 1;
    return v;
}

bool save_write(const char *path, int unlocked) {
    FILE *f = fopen(path, "w");
    if (!f) return false;

    bool ok = fprintf(f, "%d\n", unlocked) > 0;
    if (fclose(f) != 0) ok = false;         // fclose flushes the data to disk
    return ok;
}
