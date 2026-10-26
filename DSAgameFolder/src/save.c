#include "save.h"
#include <stdio.h>

#define SAVE_PATH "save.txt"

int save_load(int num_levels) {
    FILE *f = fopen(SAVE_PATH, "r");
    if (!f) return 0;                       // first run: no save yet

    int v = 0;
    if (fscanf(f, "%d", &v) != 1) v = 0;    // empty or garbage file
    fclose(f);

    if (v < 0) v = 0;
    if (v >= num_levels) v = num_levels - 1;
    return v;
}

bool save_write(int unlocked) {
    FILE *f = fopen(SAVE_PATH, "w");
    if (!f) return false;
    fprintf(f, "%d\n", unlocked);
    fclose(f);
    return true;
}
