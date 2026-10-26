#include "raylib.h"
#include "level.h"
#include "game.h"
#include "ui.h"
#include "stack.h"

#define NUM_LEVELS 3

static const char *LEVEL_PATHS[NUM_LEVELS] = {
    "levels/level01.txt",
    "levels/level02.txt",
    "levels/level03.txt",
};

// Loads a level and forgets the old undo/redo history
static bool load_level(Level *lv, int index, Stack *undo, Stack *redo) {
    stack_clear(undo);
    stack_clear(redo);
    return level_load(lv, LEVEL_PATHS[index]);
}

int main(void) {
    InitWindow(800, 600, "Sokoban Studio");
    SetTargetFPS(60);

    Stack undo, redo;
    stack_init(&undo);
    stack_init(&redo);

    int current = 0;
    Level lv;
    bool loaded = load_level(&lv, current, &undo, &redo);

    while (!WindowShouldClose()) {
        bool won = loaded && game_is_won(&lv);

        if (loaded) {
            // movement is locked once the level is solved
            if (!won) {
                int dx = 0, dy = 0;
                if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) dy = -1;
                if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) dy = 1;
                if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) dx = -1;
                if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) dx = 1;

                if (dx != 0 || dy != 0) {
                    Level before = lv;
                    if (game_move(&lv, dx, dy)) {
                        stack_push(&undo, &before);
                        stack_clear(&redo);
                    }
                }
            }

            // undo / redo still work after a win
            if (IsKeyPressed(KEY_U)) {
                Level prev;
                if (stack_pop(&undo, &prev)) { stack_push(&redo, &lv); lv = prev; }
            }
            if (IsKeyPressed(KEY_Y)) {
                Level next;
                if (stack_pop(&redo, &next)) { stack_push(&undo, &lv); lv = next; }
            }
            if (IsKeyPressed(KEY_R))
                loaded = load_level(&lv, current, &undo, &redo);

            // next level
            if (won && IsKeyPressed(KEY_ENTER) && current + 1 < NUM_LEVELS) {
                current++;
                loaded = load_level(&lv, current, &undo, &redo);
            }
        }

        won = loaded && game_is_won(&lv);   // recompute after input

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (!loaded) {
            DrawText(TextFormat("Could not load %s", LEVEL_PATHS[current]),
                     40, 40, 24, RED);
        } else {
            ui_draw(&lv);
            DrawText(TextFormat("Level %d/%d", current + 1, NUM_LEVELS),
                     300, 8, 24, DARKGRAY);
            DrawText(TextFormat("U: undo (%d)   Y: redo (%d)", undo.top, redo.top),
                     40, 540, 20, GRAY);
            if (won) {
                const char *msg = (current + 1 < NUM_LEVELS)
                    ? "You win! Press Enter for the next level"
                    : "You win! All levels complete";
                DrawText(msg, 40, 565, 28, DARKGREEN);
            }
        }
        EndDrawing();
    }

    stack_free(&undo);
    stack_free(&redo);
    CloseWindow();
    return 0;
}