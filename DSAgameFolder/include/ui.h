#ifndef UI_H
#define UI_H
#include <stdbool.h>
#include "level.h"

// Draws the level centered in the play area, with the move counter and hint.
void ui_draw(const Level *lv);

// Draws only the grid, with its top-left corner at (ox, oy) and every cell
// tile x tile pixels. The level editor uses a smaller tile for big maps.
void ui_draw_grid(const Level *lv, int ox, int oy, int tile);

// Draws a button. Returns true on the frame it is clicked with the left mouse
// button. "focused" is the highlight used by the keypad / keyboard cursor.
bool ui_button(int x, int y, int w, int h, const char *label, bool focused, bool enabled);

#endif
