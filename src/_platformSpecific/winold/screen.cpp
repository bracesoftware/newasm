// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace __SCREEN_MGR
    {
        static HANDLE hOldScreen;
        static HANDLE hNewScreen;

        //creds: stackoverflow
        FORCE_INLINE inline void open()
        {
            hOldScreen = GetStdHandle(STD_OUTPUT_HANDLE);
            
            hNewScreen = CreateConsoleScreenBuffer(
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                CONSOLE_TEXTMODE_BUFFER,
                NULL
            );

            if(hNewScreen != INVALID_HANDLE_VALUE)
            {
                SetConsoleActiveScreenBuffer(hNewScreen);
                COORD coord = {0, 0};
                SetConsoleCursorPosition(hNewScreen, coord);
            }
            return;
        }

        FORCE_INLINE inline void write(const std::string& text)
        {
            DWORD written;
            WriteConsoleA(hNewScreen, text.c_str(), (DWORD)text.length(), &written, NULL);
        }

        FORCE_INLINE inline std::string read()
        {
            std::string input;
            std::getline(std::cin, input);
            return input;
        }

        FORCE_INLINE inline void close()
        {
            if(hOldScreen != INVALID_HANDLE_VALUE)
            {
                SetConsoleActiveScreenBuffer(hOldScreen);
                CloseHandle(hNewScreen);
            }
            return;
        }
    }
}
