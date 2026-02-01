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
    #error haha 
#endif

namespace newasm
{
    namespace syscalls
    {
        namespace txtop
        {
            std::string formatted_string;
            void main()
            {
                return;
            }
            namespace impl
            {
                std::string quotes = static_cast<std::string>("\"");
                int concat()
                {
                    newasm::mem::regs::tlr = quotes+newasm::header::functions::remq(newasm::mem::regs::tlr) + 
                    newasm::header::functions::remq(newasm::mem::regs::stl)+quotes;
                    return 1;
                }
                int trim()
                {
                    newasm::mem::regs::tlr = quotes + newasm::header::functions::trim(newasm::header::functions::remq(newasm::mem::regs::tlr)) + quotes;
                    return 1;
                }

                void tokenize()
                {
                    auto addq = [](std::string& str) -> std::string {
                        str = quotes + str + quotes;
                        return str;
                    };

                    std::string string__ = newasm::header::functions::trim(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    std::string tuple__, token_temp__;

                    std::vector<std::string> tokens;
                    std::istringstream iss(string__);
                    std::string token;

                    while(iss >> token)
                    {
                        tokens.push_back(token);
                    }

                    tuple__ = "(";

                    for(int i = 0; i < tokens.size(); ++i)
                    {
                        token_temp__ = addq(tokens[i]);
                        tuple__.append(token_temp__);
                        if(i + 1 == tokens.size())
                        {
                            tuple__.append(")");
                            continue;
                        }
                        if(i + 1 != tokens.size())
                        {
                            tuple__.append(",");
                            continue;
                        }
                    }

                    newasm::mem::regs::tlr.set_value(tuple__);

                    return;
                }

                // formats stuff "My name is {namespace::name}, her name is {context("key")}"
                inline void fmat(std::string& newasm_string)
                {
                    std::string to_fmat, before_brace, after_brace;
                    int brace_open, brace_close;
                    newasm::syscalls::txtop::formatted_string.clear();

                    brace_open = newasm_string.find('{');
                    brace_close = newasm_string.find('}');

                    if((brace_open == std::string::npos) or (brace_close == std::string::npos))
                    {
                        return;
                    }

                    if(!(brace_open < brace_close))
                    {
                        return;
                    }

                    to_fmat = newasm_string.substr(brace_open + 1, brace_close - brace_open - 1);
                    before_brace = newasm_string.substr(0, brace_open);
                    after_brace = newasm_string.substr(brace_close + 1);

                    //std::cout << "before_brace: " << before_brace << std::endl;
                    //std::cout << "to_fmat: " << to_fmat << std::endl;
                    //std::cout << "after_brace: " << after_brace << std::endl;

                    to_fmat = newasm::header::functions::trim(to_fmat);
                    newasm::runtime::functions::parse(to_fmat);

                    if(newasm::header::functions::istext(to_fmat))
                    {
                        to_fmat = newasm::header::functions::remq(to_fmat);
                    }

                    newasm_string = before_brace + to_fmat + after_brace;
                    if(newasm_string.find('{') && newasm_string.find('}'))
                    {
                        fmat(newasm_string);
                    }
                    return;
                }
            }
        }
    }
}