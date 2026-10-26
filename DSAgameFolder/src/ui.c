#include "ui.h"
#include "raylib.h"

#define TILE 48
#define OFFSET_X 40
#define OFFSET_Y 40

void ui_draw(const Level *lv) {
    for (int y = 0; y < lv->height; y++) {
        for (int x = 0; x < lv->width; x++) {
            char c = lv->grid[y][x];
            int px = OFFSET_X + x * TILE;
            int py = OFFSET_Y + y * TILE;

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
    DrawText(TextFormat("Moves: %d", lv->moves), OFFSET_X, 8, 24, DARKGRAY);
    DrawText("Arrows/WASD: move   R: restart", OFFSET_X,
             OFFSET_Y + lv->height * TILE + 12, 20, GRAY);
}
