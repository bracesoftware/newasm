/*

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/

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
