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

#define __newasm__experimental_file 1

namespace newasm
{
    class procline_insert
    {
        public:
        int processline(std::string &ln)
        {
            return newasm::procline(ln);
        }
    };

    int operator<<(newasm::procline_insert &obj, std::string &ln)
    {
        obj.processline(ln);
        return 1;
    }
    namespace setup
    {
        namespace args
        {
            const int ver = 1;
            const int input = 2;
            const int help = 3;
            const int repl = 4;
            const int extra = 5;
            const int cnpf = 6;
            const int tests = 7;

            std::unordered_map<int, std::string> arg_map = {
                {ver, "-ver"},
                {input, "-input"},
                {help, "-help"},
                {repl, "-repl"},
                {extra, "-extra"},
                {cnpf, "-newproj"},
                {tests, "-tests"}
            };
        }
    }
    newasm::procline_insert process;
}