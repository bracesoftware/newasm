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

// little preprocessor

namespace newasm
{
    namespace compiler
    {
        static const int def = 1;

        static const std::unordered_map<std::string, int> instructions = {
            {"def", def}
        };

        namespace meta
        {
            std::unordered_map<std::string, std::string> defines;
        }

        std::string parse_def(std::string suf)
        {
            std::string tobeparsed = newasm::header::functions::trim(suf);
            auto it = newasm::compiler::meta::defines.find(tobeparsed);
            if(it != newasm::compiler::meta::defines.end())
            {
                tobeparsed = newasm::compiler::meta::defines.at(tobeparsed);
            }
            return tobeparsed;
        }

        void process_comptiso(std::string ins, std::string arg1, std::string arg2)
        {
            ins = newasm::header::functions::trim(ins);
            arg1 = newasm::header::functions::trim(ins);
            arg2 = newasm::header::functions::trim(ins);

            auto it = newasm::compiler::instructions.find(ins);
            if(it == newasm::compiler::instructions.end())
            {
                return;
            }
            switch(it->second)
            {
                case newasm::compiler::def:
                {
                    newasm::compiler::meta::defines[arg1] = arg2;
                    return;
                }
            }
            return;
        }
    }
}