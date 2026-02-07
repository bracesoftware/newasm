// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#include <unistd.h>
#include <termios.h>

namespace newasm
{
    namespace _compat
    {
        char getch()
        {
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            char ch = 0;
            read(STDIN_FILENO, &ch, 1);

            tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore
            return ch;
        }
    }
}
