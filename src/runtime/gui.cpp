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

namespace newasm::runtime::gui {

    enum class Key { None, Enter, Left, Right, Tab };

    inline void SetupTerminal()
    {
        #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
        #endif
    }

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

    Key GetKeyPress()
    {
        #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
        int ch = _getch();
        if (ch == 13) return Key::Enter;
        if (ch == 9) return Key::Tab;
        if (ch == 0 || ch == 224) { // Windows strelice šalju dva bajta
            int ext = _getch();
            if (ext == 75) return Key::Left;
            if (ext == 77) return Key::Right;
        }
        return Key::None;
        #else
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        int ch = getchar();
        Key result = Key::None;
        
        if (ch == 10) result = Key::Enter;
        else if (ch == 9) result = Key::Tab;
        else if (ch == 27) { // ANSI Escape sekvenca za strelice (ESC [ C/D)
            if (getchar() == '[') {
                int arrow = getchar();
                if (arrow == 'D') result = Key::Left;
                if (arrow == 'C') result = Key::Right;
            }
        }
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return result;
        #endif
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

    inline std::string CenterText(const std::string& text, int width)
    {
        if (static_cast<int>(text.length()) >= width) return text.substr(0, width);
        int padding = (width - text.length()) / 2;
        std::string result(padding, ' ');
        result += text;
        result.append(width - result.length(), ' ');
        return result;
    }

    inline int CreateBox(const std::string& title, const std::string& message, const std::vector<std::string>& buttons)
    {
        int termWidth, termHeight;
        GetTerminalSize(termWidth, termHeight);

        const int boxWidth = 50;
        int startX = (termWidth - boxWidth) / 2;
        if (startX < 1) startX = 1;
        std::vector<std::string> messageLines = WrapText(message, boxWidth - 8);
                const int boxHeight = messageLines.size() + 6; 
        int startY = (termHeight - boxHeight) / 2;
        if (startY < 1) startY = 1;

        auto moveCursor = [](int x, int y) {
            std::cout << "\033[" << y << ";" << x << "H";
        };

        std::cout << "\033[?1049h\033[?25l"; // Alternate buffer, sakrij kursor
        
        // Oboj pozadinu preko cijelog ekrana u plavo (kao klasični Linux TUI)
        std::cout << "\033[44m\033[2J"; 

        int selectedIndex = 0;
        int numButtons = buttons.size();
        bool running = true;

        // Glavna Event Petlja
        while(running)
        {
            for(int i = 0; i < boxHeight; ++i)
            {
                moveCursor(startX, startY + i);
                
                if(i == 0)
                {
                    // Title bar
                    std::cout << "\033[47;30m" << CenterText(" " + title, boxWidth) << "\033[0m\033[44m";
                } 
                else if(i == boxHeight - 2)
                {
                    std::cout << "\033[47;30m";
                    std::string btnRow = "";
                    
                    int totalBtnLen = 0;
                    for(const auto& btn : buttons) totalBtnLen += btn.length() + 4; // "[ txt ]"
                    int pad = (boxWidth - totalBtnLen) / 2;
                    
                    for(int p = 0; p < pad; ++p) std::cout << " ";
                    
                    for(int b = 0; b < numButtons; ++b)
                    {
                        if(b == selectedIndex)
                        {
                            std::cout << "\033[40;37m[ " << buttons[b] << " ]\033[47;30m";
                        }
                        else
                        {
                            std::cout << "[ " << buttons[b] << " ]";
                        }
                    }
                    
                    for(int p = pad + totalBtnLen; p < boxWidth; ++p) std::cout << " ";
                    std::cout << "\033[0m\033[44m";
                } 
                else if(i >= 2 && i < 2 + (int)messageLines.size())
                {
                    std::cout << "\033[47;30m" << CenterText(messageLines[i - 2], boxWidth) << "\033[0m\033[44m";
                }
                else
                {
                    std::cout << "\033[47;30m" << std::string(boxWidth, ' ') << "\033[0m\033[44m";
                }
            }
            std::cout << std::flush;

            // Čekanje i obrada unosa
            Key k = GetKeyPress();
            if(k == Key::Right || k == Key::Tab)
            {
                selectedIndex = (selectedIndex + 1) % numButtons;
            }
            else if(k == Key::Left)
            {
                selectedIndex = (selectedIndex - 1 + numButtons) % numButtons;
            }
            else if(k == Key::Enter)
            {
                running = false;
            }
        }

        std::cout << "\033[?1049l\033[?25h" << std::flush;
        return selectedIndex;
    }
}

int main() {
    CustomTUI::SetupTerminal();
    std::cout << "Lmao" << std::endl;
    
    std::string longMessage = "Ovo je jedan vrlo dugacak string koji testira algoritam za prelamanje teksta. "
                              "Sistem ce izracunati duzinu i automatski razbiti ovaj tekst u vise linija tako "
                              "da savrseno stane u nas okvir od 50 karaktera sirine, a visina okvira ce se prilagoditi!";
                              
    std::vector<std::string> buttons = {"Prihvati", "Odbij", "Odgodi"};

    int result = CustomTUI::CreateBox("Sistemska Konfiguracija", longMessage, buttons);

    std::cout << "Dialog zatvoren. Korisnik je odabrao opciju: " << buttons[result] << " (Index " << result << ")\n";

    return 0;
}