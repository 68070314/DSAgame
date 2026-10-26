#ifndef SERIAL_H
#define SERIAL_H
#include <stdbool.h>

// Opens a serial port such as "COM3" at the given baud rate (8N1).
// Returns false if the port is missing or already in use.
bool serial_open(const char *port, int baud);

// Returns the next received byte, or -1 if nothing is waiting. Never blocks.
int serial_read_key(void);

bool serial_is_open(void);
void serial_close(void);

#endif
