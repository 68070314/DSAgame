// Windows serial port access.
// NOTE: windows.h clashes with raylib.h (both define things like Rectangle and
// CloseWindow), so this file must never include raylib.h. serial.h is clean.
#include "serial.h"
#include <windows.h>
#include <stdio.h>

static HANDLE g_port = INVALID_HANDLE_VALUE;

bool serial_open(const char *port, int baud) {
    // the "\\.\" prefix is required for COM10 and above, and harmless below that
    char path[32];
    snprintf(path, sizeof(path), "\\\\.\\%s", port);

    g_port = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                         OPEN_EXISTING, 0, NULL);
    if (g_port == INVALID_HANDLE_VALUE) return false;   // missing, or in use

    DCB dcb;
    ZeroMemory(&dcb, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(g_port, &dcb)) { serial_close(); return false; }

    dcb.BaudRate = (DWORD)baud;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    // Boards with native USB (Uno R4, Leonardo) can stay silent until the PC
    // raises DTR, so switch it on explicitly. Harmless on a classic Uno.
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    if (!SetCommState(g_port, &dcb)) { serial_close(); return false; }

    // MAXDWORD / 0 / 0 makes ReadFile return immediately with whatever has
    // arrived, so the game loop never waits for the Arduino
    COMMTIMEOUTS timeouts;
    ZeroMemory(&timeouts, sizeof(timeouts));
    timeouts.ReadIntervalTimeout = MAXDWORD;
    if (!SetCommTimeouts(g_port, &timeouts)) { serial_close(); return false; }

    return true;
}

int serial_read_key(void) {
    if (g_port == INVALID_HANDLE_VALUE) return -1;

    unsigned char c;
    DWORD got = 0;
    if (ReadFile(g_port, &c, 1, &got, NULL) && got == 1) return c;
    return -1;
}

bool serial_is_open(void) {
    return g_port != INVALID_HANDLE_VALUE;
}

void serial_close(void) {
    if (g_port != INVALID_HANDLE_VALUE) {
        CloseHandle(g_port);
        g_port = INVALID_HANDLE_VALUE;
    }
}
