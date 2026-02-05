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


namespace newasm
{
    namespace header
    {
        namespace functions
        {
            inline std::pair<bool, int> issizeof(const std::string& str)
            {
                std::string opr;
                //std::cout << "\t" << "STR IN ISSIZEOF : `" << str << "`\n";
                if(str.size() > 2 && str[0] == '$' && str[1] == '-')
                {
                    opr = newasm::header::functions::trim(str.substr(2));

                    // dobijamo &lol
                    newasm::runtime::functions::parse(opr);

                    //newasm::progwin::api::cout("issizeof()::we_got .. -> " + opr);
                    if(newasm::header::functions::isref(opr))
                    {
                        opr = newasm::header::functions::remamp(opr);
                    
                        auto it = newasm::variables::ids.find(opr);
                        if(it == newasm::variables::ids.end())
                        {
                            //std::cout << "JEL SE RADI O OVOM? -----> " <<opr<< std::endl;
                            newasm::terminate(newasm::exit_codes::invalid_memacc);
                            return {false, 0};
                        }
                        if(it->second.type == newasm::datatypes::number) return {true, 4};
                        if(it->second.type == newasm::datatypes::decimal) return {true, 4};
                        if(it->second.type == newasm::datatypes::character) return {true, 1};
                        if(it->second.type == newasm::datatypes::text)
                        {
                            std::string buf;
                            buf = newasm::hardware::randAccessMem.peek<std::string>(it->second.addr);
                            return {true, buf.size() + 4};
                        }
                    }
                    else
                    {
                        if(newasm::header::functions::istext(opr))
                        {
                            std::string buf;
                            buf = newasm::header::functions::remq(opr);
                            return {true, buf.size()};
                        }
                    }

                }
                //std::cout << "\t" << "fail : `" << str << opr << "`\n";
                return {false, 0};
            }
        }
    }
}