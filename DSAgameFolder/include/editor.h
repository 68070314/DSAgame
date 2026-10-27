#ifndef EDITOR_H
#define EDITOR_H
#include <stdbool.h>
#include "input.h"
#include "level.h"

// What a tool does to the cell under the cursor
typedef enum {
    TOOL_WALL,      // toggle a wall
    TOOL_BOX,       // toggle a box
    TOOL_GOAL,      // toggle a goal
    TOOL_START,     // move the player start here
    TOOL_ERASE      // clear the cell
} Tool;

typedef struct {
    Level lv;           // the level being edited
    int index;          // level file number (0-based): 0 is level01.txt
    bool is_new;        // true until the level has been saved once
    int cx, cy;         // cursor cell
    Tool brush;         // tool a mouse click applies
    bool dirty;         // unsaved changes
    bool back_armed;    // true after the "unsaved changes" warning was shown
    bool status_error;  // status message is an error (red) or a note (green)
    char status[96];
} Editor;

typedef enum {
    EDITOR_STAY,
    EDITOR_EXIT         // the player left the editor
} EditorResult;

// Starts a blank new level that will be saved as level number index + 1.
void editor_new(Editor *ed, int index);

// Opens an existing level file for editing.
void editor_edit(Editor *ed, int index);

// Handles input and draws the editor. Call once per frame between
// BeginDrawing() and EndDrawing().
EditorResult editor_run(Editor *ed, Command cmd);

#endif
