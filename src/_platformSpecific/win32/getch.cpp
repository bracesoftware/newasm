// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#include <windows.h>

namespace newasm
{
    namespace _compat
    {
        char getch()
        {
            HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
            if (hInput == INVALID_HANDLE_VALUE) return 0;

            DWORD mode = 0;
            GetConsoleMode(hInput, &mode);
            SetConsoleMode(hInput, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

            INPUT_RECORD record;
            DWORD read;
            char ch = 0;

            while(true)
            {
                if (ReadConsoleInputA(hInput, &record, 1, &read))
                {
                    if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
                    {
                        ch = record.Event.KeyEvent.uChar.AsciiChar;
                        break;
                    }
                }
            }

            SetConsoleMode(hInput, mode); // restore
            return ch;
        }
    }
}
