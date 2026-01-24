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

// get shi from go
extern "C"
{
    void show_console(char* title);
    void console_out(char* text);
    char* console_in();
    void close_console();
}


namespace newasm
{
    class Console
    {
        public:
        inline static void show(std::string text)
        {
            show_console(const_cast<char*>(text.c_str()));
            return;
        }
        inline static void out(std::string text)
        {
            console_out(const_cast<char*>(text.c_str()));
            return;
        }
        inline static std::string in()
        {
            char* str = console_in();
            std::string buf(str);
            free_string(str);
            return buf;
        }
        inline static void close()
        {
            close_console();
            return;
        }
    };
}