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
    namespace common
    {
        std::vector<std::string> tokenize(const std::string line)
        {
            //newasm::header::functions::dbg(static_cast<std::string>("Tokenizing - \"") + line + static_cast<std::string>("\""));
            char delim = '0';
            int loc = -1;
            std::vector<std::string> tokens;
            std::vector<std::string> temp;
            std::string temp2;
            std::string code = newasm::header::functions::trim(line);
            if(newasm::header::functions::isalphanum(code))
            {
                tokens.push_back(code);
            }
            else if(newasm::header::functions::hasnull(code, delim, loc))
            {
                if(!newasm::header::functions::isbetween(code, delim, '\"'))
                {
                    temp.clear(); // idk just wanna be safe lmao
                    temp = newasm::header::functions::split_fixed(code, delim);
                    tokens.push_back(newasm::header::functions::trim(temp[0]));
                    temp2 = temp[1];
                    if(temp2.find(',') != std::string::npos)
                    {
                        if(!newasm::header::functions::isbetween(temp2, ',', '\"'))
                        {
                            temp.clear();
                            temp = newasm::header::functions::split_fixed(temp2, ',');
                            tokens.push_back(newasm::header::functions::trim(temp[0]));
                            tokens.push_back(newasm::header::functions::trim(temp[1]));
                            temp2.clear();
                        }
                    }
                    if(!temp2.empty())
                    {
                        tokens.push_back(newasm::header::functions::trim(temp2));
                    }
                }
            }
            for(int i = 0; i < tokens.size(); i++)
            {
                //std::cout << i << " : '" << tokens.at(i) << "'\n"; 
            }
            return tokens;
        }
    }
}