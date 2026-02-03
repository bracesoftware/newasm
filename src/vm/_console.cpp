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


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif
/*
// get shi from go
extern "C"
{
    void show_console(char* title);
    void console_out(char* text);
    char* console_in();
    void close_console();
}

*/

// NOTE: THIS FILE IS A WRAPPER FOR CONSOLE API (THIS IS JUST PSEUDO CODE)
// I'M PLANNING TO ADD A FAST GUI, SO APPS LOOK LIKE THEY ARE APPS
// FOR NOW WE JUST DO THESE

#ifndef USING_SDL_FINALLY
#define USING_SDL_FINALLY 0
#endif

#if USING_SDL_FINALLY == 1
extern "C"
{
    void openConsoleWindow(const char* title);
    void renderScreen();
    void inputFromConsole(char* buffer, int size);
    void printToConsole(const char* text);
    void closeConsoleWindow();
    void cls();
}
#endif
namespace newasm
{
    static bool consoleOpen = false;
    class Console
    {
        public:
        inline static void flush()
        {
            #if USING_SDL_FINALLY == 1
            renderScreen();
            #endif
            return;
        }

        inline static void show(const std::string& text)
        {
            //show_console(const_cast<char*>(text.c_str()));
            if(newasm::consoleOpen)
            {
                return;
            }
            newasm::consoleOpen = true;

            #if USING_SDL_FINALLY == 1
            openConsoleWindow(text.c_str());
            #endif
            return;
        }
        inline static void out(const std::string& text)
        {
            if(!newasm::consoleOpen)
            {
                return;
            }

            //console_out(const_cast<char*>(text.c_str()));
            #if USING_SDL_FINALLY == 1
            printToConsole(text.c_str());
            #endif
            #if USING_SDL_FINALLY == 0
            std::cout << text;
            #endif
            return;
        }
        inline static std::string in()
        {
            if(!newasm::consoleOpen)
            {
                return std::string("nil");
            }
            #if USING_SDL_FINALLY == 1
            char input[128] = {0};
            inputFromConsole(input, sizeof(input));
            std::string line(input);
            return line;
            #endif
            #if USING_SDL_FINALLY == 0
            std::string line;
            std::getline(std::cin, line);
            return line;
            #endif
        }

        inline static void close()
        {
            if(!newasm::consoleOpen)
            {
                return;
            }
            //close_console();
            newasm::consoleOpen = false;

            #if USING_SDL_FINALLY == 1
            closeConsoleWindow();
            #endif
            return;
        }

        inline static void cls()
        {
            if(!newasm::consoleOpen)
            {
                return;
            }

            #if USING_SDL_FINALLY == 1
            cls();
            #endif
            #if USING_SDL_FINALLY == 0
                #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
                    std::system("cls");
                #elif _NEWASM_OS == _NEWASM_OS_linux
                    std::system("clear");
                #elif _NEWASM_OS == _NEWASM_OS_android
                    std::cout << "\033[2J\033[H";
                #endif
            #endif
            return;
        }
    };
}