#include "ui.h"
#include "raylib.h"

#define TILE 48
#define SCREEN_W 800
#define AREA_TOP 50
#define AREA_BOTTOM 470

void ui_draw(const Level *lv) {
    int map_w = lv->width * TILE;
    int map_h = lv->height * TILE;

    int ox = (SCREEN_W - map_w) / 2;
    int oy = AREA_TOP + (AREA_BOTTOM - AREA_TOP - map_h) / 2;
    if (ox < 0) ox = 0;
    if (oy < AREA_TOP) oy = AREA_TOP;

    for (int y = 0; y < lv->height; y++) {
        for (int x = 0; x < lv->width; x++) {
            char c = lv->grid[y][x];
            int px = ox + x * TILE;
            int py = oy + y * TILE;

            if (c == '#') {
                DrawRectangle(px, py, TILE, TILE, DARKGRAY);
                continue;
            }
            DrawRectangle(px, py, TILE, TILE, LIGHTGRAY);        // floor
            DrawRectangleLines(px, py, TILE, TILE, GRAY);

            if (c == '.' || c == '*' || c == '+')                // goal marker
                DrawCircle(px + TILE / 2, py + TILE / 2, 6, RED);
            if (c == '$' || c == '*')                            // box
                DrawRectangle(px + 6, py + 6, TILE - 12, TILE - 12,
                              c == '*' ? GREEN : BROWN);
            if (c == '@' || c == '+')                            // player
                DrawCircle(px + TILE / 2, py + TILE / 2, TILE / 3, BLUE);
        }
    }

    // move counter above the map, hint below it, both lined up with the map
    DrawText(TextFormat("Moves: %d", lv->moves), ox, oy - 30, 24, DARKGRAY);
    DrawText("Arrows/WASD: move   R: restart", ox, oy + map_h + 12, 20, GRAY);
}