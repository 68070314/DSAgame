#include "level.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool level_load(Level *lv, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return false;

    memset(lv->grid, ' ', sizeof(lv->grid));
    lv->width = lv->height = lv->moves = 0;
    lv->player_x = lv->player_y = -1;

    char line[256];
    while (lv->height < MAX_H && fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = '\0';        // strip newline
        int len = (int)strlen(line);
        if (len > MAX_W) len = MAX_W;
        for (int x = 0; x < len; x++) {
            lv->grid[lv->height][x] = line[x];
            if (line[x] == '@' || line[x] == '+') {
                lv->player_x = x;
                lv->player_y = lv->height;
            }
        }
        if (len > lv->width) lv->width = len;
        lv->height++;
    }
    fclose(f);
    return lv->height > 0 && lv->player_x >= 0;    // must have a player
}