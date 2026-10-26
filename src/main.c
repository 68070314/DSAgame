#include "../include/raylib.h"
#include <stdbool.h>
#include <string.h>

#define TILE 48
#define ROWS 7
#define COLS 7
#define MAX_HIST 1000

static const char *LEVEL[ROWS] = {
    "#######",
    "#     #",
    "#  $  #",
    "#     #",
    "#  .  #",
    "#     #",
    "#######",
};

// Everything that changes during play, bundled so it can be copied as one snapshot
typedef struct {
    char map[ROWS][COLS + 1];
    int px, py, moves;
} State;

// A stack: last in, first out
typedef struct {
    State items[MAX_HIST];
    int top;                    // number of items stored
} Stack;

static Stack undoStack, redoStack;   // static = zero-initialised, top starts at 0

static void stack_push(Stack *s, const State *st) {
    if (s->top == MAX_HIST) {                       // full: drop the oldest snapshot
        memmove(&s->items[0], &s->items[1], sizeof(State) * (MAX_HIST - 1));
        s->top--;
    }
    s->items[s->top++] = *st;
}

static bool stack_pop(Stack *s, State *out) {
    if (s->top == 0) return false;                  // nothing to pop
    *out = s->items[--s->top];
    return true;
}

static void reset_level(State *s) {
    for (int y = 0; y < ROWS; y++) strcpy(s->map[y], LEVEL[y]);
    s->px = 1; s->py = 1; s->moves = 0;
}

static int boxes_left(const State *s) {
    int n = 0;
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            if (s->map[y][x] == '$') n++;
    return n;
}

// Tries to move the player. Returns true only if something actually changed.
static bool try_move(State *s, int dx, int dy) {
    int nx = s->px + dx, ny = s->py + dy;
    char target = s->map[ny][nx];

    if (target == '#') return false;

    if (target == '$' || target == '*') {
        int bx = nx + dx, by = ny + dy;
        char beyond = s->map[by][bx];
        if (beyond != ' ' && beyond != '.') return false;   // wall or another box
        s->map[by][bx] = (beyond == '.') ? '*' : '$';
        s->map[ny][nx] = (target == '*') ? '.' : ' ';
    }
    s->px = nx; s->py = ny; s->moves++;
    return true;
}

int main(void) {
    State cur;
    reset_level(&cur);

    InitWindow(800, 600, "Sokoban Studio");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // 1. Input: movement
        int dx = 0, dy = 0;
        if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) dy = -1;
        if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) dy = 1;
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) dx = -1;
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) dx = 1;

        if (dx != 0 || dy != 0) {
            State before = cur;                     // snapshot BEFORE the move
            if (try_move(&cur, dx, dy)) {
                stack_push(&undoStack, &before);    // only save moves that happened
                redoStack.top = 0;                  // a new move erases the redo future
            }
        }

        // 2. Input: undo / redo / restart
        if (IsKeyPressed(KEY_U)) {
            State prev;
            if (stack_pop(&undoStack, &prev)) {
                stack_push(&redoStack, &cur);       // current state becomes redo-able
                cur = prev;
            }
        }
        if (IsKeyPressed(KEY_Y)) {
            State next;
            if (stack_pop(&redoStack, &next)) {
                stack_push(&undoStack, &cur);
                cur = next;
            }
        }
        if (IsKeyPressed(KEY_R)) {
            reset_level(&cur);
            undoStack.top = 0;
            redoStack.top = 0;
        }

        // 3. Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                int px = 40 + x * TILE;
                int py = 40 + y * TILE;
                char c = cur.map[y][x];

                if (c == '#') {
                    DrawRectangle(px, py, TILE, TILE, DARKGRAY);
                    continue;
                }
                DrawRectangle(px, py, TILE, TILE, LIGHTGRAY);
                DrawRectangleLines(px, py, TILE, TILE, GRAY);

                if (c == '.' || c == '*')
                    DrawCircle(px + TILE / 2, py + TILE / 2, 6, RED);
                if (c == '$' || c == '*')
                    DrawRectangle(px + 6, py + 6, TILE - 12, TILE - 12,
                                  c == '*' ? GREEN : BROWN);
            }
        }

        DrawCircle(40 + cur.px * TILE + TILE / 2,
                   40 + cur.py * TILE + TILE / 2, TILE / 3, BLUE);

        DrawText(TextFormat("Moves: %d   Undo: %d   Redo: %d",
                 cur.moves, undoStack.top, redoStack.top), 40, 8, 24, DARKGRAY);
        DrawText("Arrows/WASD: move   U: undo   Y: redo   R: restart",
                 40, 40 + ROWS * TILE + 12, 20, GRAY);
        if (boxes_left(&cur) == 0)
            DrawText("You win!", 40, 40 + ROWS * TILE + 44, 32, DARKGREEN);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}