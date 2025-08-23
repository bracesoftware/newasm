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

#if 0
	extern "C" void testfunc()
	{
		return;
	}
#endif

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    int process_s_(bool &valid, std::string wholeline, std::string stat, std::string arg);
    int process_s(std::string& section);
    namespace user
    {
        int udb_hash(const std::string& input);
    }
    int terminate(int exit_code);
    void async(std::string name);
    namespace header
    {
        namespace functions
        {
            void krnl(std::string text);
            //void parseRegDeref(std::string& arg);
            std::pair<bool, int> issizeof(const std::string& str);
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
    namespace compiler
    {
        struct lineData
        {
            std::string raw;
            int type;
            std::vector<std::string> tokens;
            std::string other;
        };
    }
    int procline(newasm::compiler::lineData& line);
    int procline(std::string& text);
    void tokenize(std::string str);
    namespace impl
    {
        std::string eval(std::string str);
    }
}