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

namespace newasm
{
    int terminate(int exit_code);
    namespace header
    {
        namespace col
        {
            const std::string red = "\033[31m";
            const std::string green = "\033[32m";
            const std::string yellow = "\033[33m";
            const std::string blue = "\033[34m";
            const std::string magenta = "\033[35m";
            const std::string cyan = "\033[36m";
            const std::string gray = "\033[90m";

            const std::string reset = "\033[0m";
        }
        namespace style
        {
            const std::string underline = "\033[4m";
            const std::string bold = "\033[1m";
        }

        namespace functions
        {
            void krnl(std::string text);
        }
    }
    namespace common
    {
        std::vector<std::string> tokenize(const std::string line);
    }
    namespace runtime
    {
        int version;
        namespace functions
        {
            void parse(std::string& suf);
        }
    }
    int procline(std::string &line);
    void tokenize(std::string str);
    namespace impl
    {
        std::string eval(std::string str);
    }
}