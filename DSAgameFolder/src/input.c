#include "input.h"

// Keypad layout (4x4 membrane keypad):
//
//      1   [2 up]     3    [A undo]
//   [4 left] 5   [6 right] [B redo]
//   [7 prev] [8 down] [9 next] [C restart]
//      *    0   [# confirm]   D
//
// Changing the layout only means editing this table. The Arduino just sends the
// raw key, so it never needs to be reflashed.
Command input_from_key(int c) {
    switch (c) {
        case '2': return CMD_UP;
        case '8': return CMD_DOWN;
        case '4': return CMD_LEFT;
        case '6': return CMD_RIGHT;
        case 'A': return CMD_UNDO;
        case 'B': return CMD_REDO;
        case 'C': return CMD_RESTART;
        case '#': return CMD_CONFIRM;
        case '7': return CMD_PREV;
        case '9': return CMD_NEXT;
        default:  return CMD_NONE;
    }
}
