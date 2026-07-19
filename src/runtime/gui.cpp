// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
#include <windows.h>
#include <conio.h>
#elif _NEWASM_OS == _NEWASM_OS_linux
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

//ai written at first but reworked
namespace newasm::runtime::gui
{
    #if _NEWASM_OS == _NEWASM_OS_windows_old || _NEWASM_OS == _NEWASM_OS_windows
    static HANDLE OriginalBuf = GetStdHandle(STD_OUTPUT_HANDLE);
    static HANDLE PopupBuf = NULL;

    inline void EnterPopupMode()
    {
        #if _NEWASM_OS == _NEWASM_OS_linux
        //magic code
        std::cout << "\033[?1049h\033[?25l" << std::flush;
        #else
        PopupBuf = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, CONSOLE_TEXTMODE_BUFFER, NULL
        );

        #if _NEWASM_OS != _NEWASM_OS_windows_old
        DWORD sm = 0;
        GetConsoleMode(PopupBuf, &sm);
        sm |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(PopupBuf, sm);
        #endif
        
        CONSOLE_CURSOR_INFO cursorInfo = {100, FALSE};
        SetConsoleCursorInfo(PopupBuf, &cursorInfo);
        
        SetConsoleActiveScreenBuffer(PopupBuf);
        SetStdHandle(STD_OUTPUT_HANDLE, PopupBuf);

        freopen("CONOUT$", "w", stdout);
        std::cout.clear();
        #endif
        return;
    }

    inline void ExitPopupMode()
    {
        #if _NEWASM_OS == _NEWASM_OS_linux
        //yet more magic
        std::cout << "\033[?1049l\033[?25h" << std::flush;
        #else
        SetConsoleActiveScreenBuffer(OriginalBuf);
        SetStdHandle(STD_OUTPUT_HANDLE, OriginalBuf);

        freopen("CONOUT$", "w", stdout);
        std::cout.clear();
        
        if(PopupBuf)
        {
            CloseHandle(PopupBuf);
            PopupBuf = NULL;
        }
        #endif
        return;
    }


    class ConShapshot final
    {
        public COORD size;
        COORD cursor;
        std::vector<CHAR_INFO> buffer;
    };

    inline ConShapshot SaveConsole()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        
        SMALL_RECT region = csbi.srWindow;
        COORD size = { 
            (short)(region.Right - region.Left + 1),
            (short)(region.Bottom - region.Top + 1)
        };
        
        ConShapshot snapshot;
        snapshot.size = size;
        snapshot.cursor = csbi.dwCursorPosition;
        snapshot.buffer.resize(size.X * size.Y);
        
        ReadConsoleOutput(hOut, snapshot.buffer.data(), size, {0,0}, &region);
        return snapshot;
    }

    inline void RestoreConsole(const ConShapshot& snapshot)
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SMALL_RECT region = {
            0, 0,
            static_cast<SHORT>(snapshot.size.X - 1),
            static_cast<SHORT>((short)snapshot.size.Y - 1)
        };
        WriteConsoleOutput(hOut, snapshot.buffer.data(), snapshot.size, {0,0}, &region);
        SetConsoleCursorPosition(hOut, snapshot.cursor);
        return;
    }
    #endif

    inline void GetTerminalSize(int& width, int& height)
    {
        #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        width = w.ws_col;
        height = w.ws_row;
        #endif
    }

    const int KEY_ENTER = 1;
    const int KEY_LEFT = 2;
    const int KEY_RIGHT = 3;
    const int KEY_TAB = 4;
    const int KEY_NONE = 5;

    std::atomic<int> pressed(0);
    std::atomic<char> ch('?');
    bool killInputListener = false;

    void inputListener()
    {
        pressed = KEY_NONE;
        while(!killInputListener)
        {
            ch = newasm::_compat::getch();
            if(ch == 'A' or ch == 'a') pressed = KEY_LEFT;
            else if(ch == 'D' or ch == 'd') pressed = KEY_RIGHT;
            else if(ch == '\n' or ch == '\r') pressed = KEY_ENTER;
            else if(ch == '\t') pressed = KEY_TAB;
            else pressed = KEY_NONE;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    inline std::vector<std::string> WrapText(const std::string& text, int maxWidth)
    {
        std::vector<std::string> lines;
        std::istringstream words(text);
        std::string word, currentLine;

        while(words >> word)
        {
            if(currentLine.length() + word.length() + 1 > static_cast<size_t>(maxWidth))
            {
                if(!currentLine.empty()) lines.push_back(currentLine);
                currentLine = word;
            }
            else
            {
                if(!currentLine.empty()) currentLine += " ";
                currentLine += word;
            }
        }
        if(!currentLine.empty()) lines.push_back(currentLine);
        return lines;
    }

    inline std::string CenterText_OLD(const std::string& text, int width)
    {
        if(static_cast<int>(text.length()) >= width) return text.substr(0, width);
        int padding = (width - text.length()) / 2;
        std::string result(padding, ' ');
        result += text;
        result.append(width - result.length(), ' ');
        return result;
    }

    inline std::string CenterText(const std::string& text, int width)
    {
        if(static_cast<int>(text.length()) >= width) 
        {
            return text.substr(0, width);
        }

        std::string result(width, ' ');
        size_t padding = (width - text.length()) / 2;
        result.replace(padding, text.length(), text);
        return result;
    }

    inline int box(const std::string& title, const std::string& message, const std::vector<std::string>& buttons)
    {
        int termWidth = 0, termHeight = 0, TW_OLD = 1, TH_OLD = 1;
        auto moveCursor = [](int x, int y) {
            #if _NEWASM_OS == _NEWASM_OS_linux
            std::cout << "\033[" << y << ";" << x << "H";
            #elif _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
            HANDLE h = (PopupBuf != NULL) ? PopupBuf : OriginalBuf;
            COORD pos = {
                (short)x,
                (short)y
            };
            SetConsoleCursorPosition(h, pos);
            #endif
        };
        const int boxWidth = 50;
        int boxHeight, startY, startX;
        static const std::string CLS = "\033[2J";
        std::vector<std::string> messageLines = WrapText(message, boxWidth - 8);
        //magic-DO NOT TOUCH! NOTE INFO IMPORTANT
        EnterPopupMode();
        std::cout << newasm::header::col::sky_blue;

        int selectedIndex = 0;
        int numButtons = buttons.size();
        bool running = true;
        std::thread listener(inputListener);
        while(running)
        {
            std::cout << newasm::header::col::sky_blue;
            GetTerminalSize(termWidth, termHeight);

            if(TW_OLD != termWidth or TH_OLD != termHeight)
            {
                NewASM::Console::cls_BARE_METAL__();
                startX = (termWidth - boxWidth) / 2;
                if(startX < 1) startX = 1;
                boxHeight = messageLines.size() + 6; 
                startY = (termHeight - boxHeight) / 2;
                if(startY < 1) startY = 1;
            }
            TW_OLD = termWidth;
            TH_OLD = termHeight;

            
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            for(int i = 0; i < boxHeight; ++i)
            {
                moveCursor(startX, startY + i);
                
                if(i == 0)
                {
                    std::cout << newasm::header::style::underline << newasm::header::col::black;
                    std::cout << newasm::header::bg::gray << CenterText(" " + title, boxWidth);
                    std::cout << newasm::header::col::reset << newasm::header::col::sky_blue;
                } 
                else if(i == boxHeight - 2)
                {
                    std::cout << newasm::header::bg_col::black_white;
                    std::string btnRow = "";
                    
                    int totalBtnLen = 0;
                    for(const auto& btn : buttons) totalBtnLen += btn.length() + 2;
                    int pad = (boxWidth - totalBtnLen - numButtons) / 2;
                    
                    for(int p = 0; p < pad; ++p) std::cout << ' ';
                    
                    for(int b = 0; b < numButtons; ++b)
                    {
                        if(b == selectedIndex)
                        {
                            std::cout << newasm::header::bg_col::white_black;
                            std::cout << '*' << buttons[b] << ' ';
                            std::cout << newasm::header::bg_col::black_white;
                        }
                        else
                        {
                            std::cout << newasm::header::bg::gray << newasm::header::col::black;
                            std::cout << ' ' << buttons[b] << ' ';
                            std::cout << newasm::header::bg_col::black_white;
                        }
                        std::cout << ' ';
                    }
                    
                    for(int p = pad + totalBtnLen; p < boxWidth - numButtons; ++p) std::cout << ' ';
                    std::cout << newasm::header::col::reset << newasm::header::col::sky_blue;
                } 
                else if(i >= 2 && i < 2 + (int)messageLines.size())
                {
                    std::cout << newasm::header::bg_col::black_white;
                    std::cout << newasm::header::col::gray << CenterText(messageLines[i - 2], boxWidth);
                    std::cout << newasm::header::col::reset << newasm::header::col::sky_blue;
                }
                else
                {
                    std::cout << newasm::header::bg_col::black_white << std::string(boxWidth, ' ');
                    std::cout << newasm::header::col::reset << newasm::header::col::sky_blue;
                }
            }

            std::string footerText = "Navigate with `A`, `D` and `Tab` keys. `Enter` to continue.";
            int footerX = startX + (boxWidth - (int)footerText.length()) / 2;
            int footerY = startY + boxHeight + 2;

            moveCursor(footerX, footerY);
            std::cout << newasm::header::col::yellow << footerText << newasm::header::col::reset;

            std::cout << std::flush;

            if(pressed == KEY_RIGHT or pressed == KEY_TAB)
            {
                selectedIndex = (selectedIndex + 1) % numButtons;
                pressed = KEY_NONE;
            }
            else if(pressed == KEY_LEFT)
            {
                selectedIndex = (selectedIndex - 1 + numButtons) % numButtons;
                pressed = KEY_NONE;
            }
            else if(pressed == KEY_ENTER)
            {
                pressed = KEY_NONE;
                killInputListener = true;
                running = false;
            }
            else if(pressed == KEY_NONE)
            {
                continue;
            }
        }
        listener.join();
        ExitPopupMode();
        return selectedIndex;
    }
}