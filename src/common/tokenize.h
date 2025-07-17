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
            //newasm::header::functions::dbg(static_cast<std::string>("\t\t\t\nTokenizing - \"") + line + static_cast<std::string>("\""));
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
                //std::cout << "\t\t\t" << i << " : '" << tokens.at(i) << "'\n"; 
            }
            return tokens;
        }
        std::vector<std::string> tokenize2(const std::string& input)
        {
            std::vector<std::string> tokens;

            size_t i = 0;
            while(i < input.size() && !std::isspace(static_cast<unsigned char>(input[i])))
            {
                i++;
            }

            if(i == input.size())
            {
                // Nema razmaka (nema whitespace)
                tokens.push_back(input);
            }
            else
            {
                // Razdvoji u dva dijela
                std::string first = input.substr(0, i);

                // preskoči sve whitespace karaktere nakon prvog
                size_t j = i;
                while(j < input.size() && std::isspace(static_cast<unsigned char>(input[j])))
                {
                    j++;
                }

                std::string second = input.substr(j);
                tokens.push_back(first);
                tokens.push_back(second);
            }

            return tokens;
        }

    }
}