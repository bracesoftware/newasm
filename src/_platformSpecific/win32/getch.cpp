// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#include <windows.h>

namespace newasm
{
    namespace _compat
    {
        inline char getch_OLD() noexcept //very useful
        {
            HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
            if(hInput == INVALID_HANDLE_VALUE)
            {
                return 0;
            }

            DWORD mode = 0;
            GetConsoleMode(hInput, &mode);
            SetConsoleMode(hInput, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

            INPUT_RECORD record;
            DWORD read;
            char ch = 0;

            while(true)
            {
                if(ReadConsoleInputA(hInput, &record, 1, &read))
                {
                    if(record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
                    {
                        ch = record.Event.KeyEvent.uChar.AsciiChar;
                        break;
                    }
                }
            }

            SetConsoleMode(hInput, mode); // restore
            return ch;
        }

        //stack overflow
        inline char getch() noexcept
        {
            HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
            DWORD numEvents = 0;
            
            GetNumberOfConsoleInputEvents(hInput, &numEvents);
            if(numEvents == 0) return 0;

            INPUT_RECORD record;
            DWORD read;
            if(ReadConsoleInputA(hInput, &record, 1, &read))
            {
                if(record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
                {
                    return record.Event.KeyEvent.uChar.AsciiChar;
                }
            }
            return 0;
        }

        // two very useful functions from stackoverflow >:)
        inline void set_cursor(int x, int y) noexcept
        {
            COORD pos = {(SHORT)x, (SHORT)y};
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        }

        inline void clear_line(int y) noexcept
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD start = {0, (SHORT)y};
            DWORD written;
            FillConsoleOutputCharacter(h, ' ', 80, start, &written);
            SetConsoleCursorPosition(h, start);
        }
    }
}
