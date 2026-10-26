#ifndef INPUT_H
#define INPUT_H

// Everything the player can ask the game to do, whatever device it came from
typedef enum {
    CMD_NONE,
    CMD_UP,
    CMD_DOWN,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_UNDO,
    CMD_REDO,
    CMD_RESTART,
    CMD_CONFIRM,    // next level after a win
    CMD_PREV,       // previous level
    CMD_NEXT        // next unlocked level
} Command;

// Maps one character sent by the Arduino keypad to a command.
// Unknown characters (including '\r' and '\n') give CMD_NONE.
Command input_from_key(int c);

#endif
