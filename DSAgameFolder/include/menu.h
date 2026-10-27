#ifndef MENU_H
#define MENU_H
#include <stdbool.h>
#include "input.h"

// What the main menu wants the game to do this frame
typedef enum {
    MENU_NONE,
    MENU_CONTINUE,      // go to level select
    MENU_NEW_GAME,
    MENU_EDITOR,        // go to the level editor's level table
    MENU_QUIT
} MenuAction;

// Special return values of menu_level_select (a level index is >= 0)
#define LEVELSEL_NONE (-1)
#define LEVELSEL_BACK (-2)
#define LEVELSEL_ADD  (-3)      // the "+" button was chosen

// Return values of menu_confirm
#define CONFIRM_PENDING (-1)    // no answer yet; 0 = No, 1 = Yes

// Draws the main menu and handles keypad / keyboard / mouse input for it.
// Continue is greyed out and cannot be picked when can_continue is false.
MenuAction menu_main(Command cmd, bool can_continue);

// Puts the level-select cursor on a level (call this when opening the screen).
void menu_level_select_reset(int cursor);

// Draws a table of level buttons and handles input.
//  - levels above `unlocked` are shown as locked and cannot be picked
//  - with allow_add, a "+" button follows the last level
// Returns the chosen level index, LEVELSEL_ADD, LEVELSEL_BACK or LEVELSEL_NONE.
int menu_level_select(Command cmd, const char *title, int num_levels,
                      int unlocked, bool allow_add);

// Draws a Yes / No question. Returns 1 for Yes, 0 for No, or CONFIRM_PENDING.
int menu_confirm(Command cmd, const char *question);

// Draws the small "Levels" button used while playing.
// Returns true on the frame it is clicked.
bool menu_levels_button(void);

#endif
