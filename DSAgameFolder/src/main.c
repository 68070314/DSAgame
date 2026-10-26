#include "raylib.h"
#include "level.h"
#include "game.h"
#include "ui.h"
#include "stack.h"
#include "save.h"
#include "input.h"
#include "serial.h"

#include <stdio.h>
#include <string.h>

#define NUM_LEVELS 3
#define BAUD_RATE 9600          // must match Serial.begin() in the Arduino sketch

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

// Reads the port name (for example COM5) from port.txt next to the exe.
// Falls back to COM3 if the file is missing.
static void read_port_name(char *out, size_t size) {
    snprintf(out, size, "COM3");
    FILE *f = fopen("port.txt", "r");
    if (!f) return;

    char buf[32];
    if (fgets(buf, sizeof(buf), f)) {
        buf[strcspn(buf, " \t\r\n")] = '\0';        // trim at first whitespace
        if (buf[0] != '\0') snprintf(out, size, "%s", buf);
    }
    fclose(f);
}

// Builds the full path of save.txt inside the folder that contains the .exe, so
// the save file is the same no matter which folder the game was started from.
static void build_save_path(char *out, size_t size) {
    const char *dir = GetApplicationDirectory();    // normally ends with a slash
    size_t n = strlen(dir);
    const char *sep = (n > 0 && (dir[n - 1] == '/' || dir[n - 1] == '\\')) ? "" : "/";
    snprintf(out, size, "%s%ssave.txt", dir, sep);
}

static Command poll_command(void) {
    int c;
    while ((c = serial_read_key()) != -1) {
        Command cmd = input_from_key(c);
        if (cmd != CMD_NONE) return cmd;
    }

    //can play with keyboard too
    if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) return CMD_UP;
    if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) return CMD_DOWN;
    if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) return CMD_LEFT;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return CMD_RIGHT;
    if (IsKeyPressed(KEY_U))     return CMD_UNDO;
    if (IsKeyPressed(KEY_Y))     return CMD_REDO;
    if (IsKeyPressed(KEY_R))     return CMD_RESTART;
    if (IsKeyPressed(KEY_ENTER)) return CMD_CONFIRM;
    if (IsKeyPressed(KEY_P))     return CMD_PREV;
    if (IsKeyPressed(KEY_N))     return CMD_NEXT;
    return CMD_NONE;
}

int main(void) {
    InitWindow(800, 600, "Sokoban Studio");
    SetTargetFPS(60);

    char save_path[512];
    build_save_path(save_path, sizeof(save_path));
    SetWindowTitle(TextFormat("Sokoban Studio - save file: %s", save_path));
    bool save_ok = true;                    // false if the last save attempt failed

    char port[32];
    read_port_name(port, sizeof(port));
    bool arduino = serial_open(port, BAUD_RATE);

    Stack undo, redo;
    stack_init(&undo);
    stack_init(&redo);

    int unlocked = save_load(save_path, NUM_LEVELS);   // highest level the player may play
    int current = unlocked;                 // resume where the player left off
    Level lv;
    bool loaded = load_level(&lv, current, &undo, &redo);

    while (!WindowShouldClose()) {
        Command cmd = poll_command();
        bool won = loaded && game_is_won(&lv);

        if (loaded) {
            // movement is locked once the level is solved
            if (!won) {
                int dx = 0, dy = 0;
                if (cmd == CMD_UP)    dy = -1;
                if (cmd == CMD_DOWN)  dy = 1;
                if (cmd == CMD_LEFT)  dx = -1;
                if (cmd == CMD_RIGHT) dx = 1;

                if (dx != 0 || dy != 0) {
                    Level before = lv;
                    if (game_move(&lv, dx, dy)) {
                        stack_push(&undo, &before);
                        stack_clear(&redo);
                    }
                }
            }

            // undo / redo still work after a win
            if (cmd == CMD_UNDO) {
                Level prev;
                if (stack_pop(&undo, &prev)) { stack_push(&redo, &lv); lv = prev; }
            }
            if (cmd == CMD_REDO) {
                Level next;
                if (stack_pop(&redo, &next)) { stack_push(&undo, &lv); lv = next; }
            }
            if (cmd == CMD_RESTART)
                loaded = load_level(&lv, current, &undo, &redo);

            // next level after a win
            if (won && cmd == CMD_CONFIRM && current + 1 < NUM_LEVELS) {
                current++;
                loaded = load_level(&lv, current, &undo, &redo);
            }

            // replay an earlier level, or go forward to one already unlocked
            if (cmd == CMD_PREV && current > 0) {
                current--;
                loaded = load_level(&lv, current, &undo, &redo);
            }
            if (cmd == CMD_NEXT && current < unlocked) {
                current++;
                loaded = load_level(&lv, current, &undo, &redo);
            }
        }

        won = loaded && game_is_won(&lv);   // recompute after input

        // winning unlocks the next level and saves immediately
        if (won && current + 1 < NUM_LEVELS && unlocked < current + 1) {
            unlocked = current + 1;
            save_ok = save_write(save_path, unlocked);
        } else if (!save_ok) {
            save_ok = save_write(save_path, unlocked);   // try again if it failed before
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (!loaded) {
            DrawText(TextFormat("Could not load %s", LEVEL_PATHS[current]),
                     40, 40, 24, RED);
        } else {
            ui_draw(&lv);
            DrawText(TextFormat("Level %d/%d", current + 1, NUM_LEVELS),
                     300, 8, 24, DARKGRAY);

            if (arduino)
                DrawText(TextFormat("Arduino: %s connected", port),
                         500, 12, 18, DARKGREEN);
            else
                DrawText(TextFormat("Arduino: %s not found (keyboard mode)", port),
                         440, 12, 18, ORANGE);

            DrawText("Keypad: 2 up  8 down  4 left  6 right", 40, 480, 18, GRAY);
            DrawText("A undo  B redo  C restart  # confirm  7 prev  9 next",
                     40, 502, 18, GRAY);
            if (save_ok)
                DrawText(TextFormat("Unlocked: %d of %d levels (auto-saved)",
                         unlocked + 1, NUM_LEVELS), 40, 520, 18, GRAY);
            else
                DrawText("SAVE FAILED - file path is in the window title",
                         40, 520, 18, RED);
            DrawText(TextFormat("Undo steps: %d   Redo steps: %d", undo.top, redo.top),
                     40, 540, 20, GRAY);

            if (won) {
                const char *msg = (current + 1 < NUM_LEVELS)
                    ? "You win! Press # (or Enter) for the next level"
                    : "You win! All levels complete";
                DrawText(msg, 40, 565, 28, DARKGREEN);
            }
        }
        EndDrawing();
    }

    serial_close();
    stack_free(&undo);
    stack_free(&redo);
    CloseWindow();
    return 0;
}
