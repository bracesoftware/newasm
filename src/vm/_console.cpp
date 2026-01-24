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


namespace newasm
{
    static bool consoleOpen = false;
    class Console
    {
        public:
        inline static void show(std::string text)
        {
            //show_console(const_cast<char*>(text.c_str()));
            if(newasm::consoleOpen)
            {
                return;
            }
            newasm::consoleOpen = true;
            return;
        }
        inline static void out(std::string text)
        {
            if(!newasm::consoleOpen)
            {
                return;
            }

            //console_out(const_cast<char*>(text.c_str()));
            std::cout << text;
            return;
        }
        inline static std::string in()
        {
            if(!newasm::consoleOpen)
            {
                return std::string("nil");
            }
            /*char* str = console_in();
            std::string buf(str);
            free_string(str);*/
            std::string line;
            std::getline(std::cin, line);
            return line;
        }
        inline static void close()
        {
            if(!newasm::consoleOpen)
            {
                return;
            }
            //close_console();
            newasm::consoleOpen = false;
            return;
        }
    };
}