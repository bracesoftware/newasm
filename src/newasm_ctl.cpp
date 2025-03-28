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
    namespace ctl
    {
        namespace data
        {
            std::string cmd;
            bool finish = false;
        }
        namespace impl
        {
            int process_c(std::string cmd)
            {
                std::vector<std::string> tokens = newasm::header::functions::split(newasm::header::functions::trim(cmd),' ');
                for(int i = 0; i < tokens.size(); i++)
                {
                    tokens[i] = newasm::header::functions::trim(tokens[i]);
                }
                if(tokens.size() == 0)
                {
                    return 1;
                }
                if(tokens.size() == 1)
                {
                    if(tokens[0] == static_cast<std::string>("exit"))
                    {
                        newasm::ctl::data::finish = true;
                        return 1;
                    }
                    if(tokens[0] == static_cast<std::string>("help"))
                    {
                        //newasm::ctl::data::finish = true;
                        return 1;
                    }
                }
                newasm::header::functions::err("Invalid command.");
                return 1;
            }
        }
        void main()
        {
            while(true)
            {
                if(newasm::ctl::data::finish)
                {
                    return;
                }
                using std::cin;
                std::cout << newasm::header::col::gray << ">>> " << newasm::header::col::reset
                << newasm::header::style::underline;
                std::getline(cin, newasm::ctl::data::cmd);
                std::cout << newasm::header::col::reset;
                newasm::ctl::impl::process_c(newasm::ctl::data::cmd);
            }

            return;
        }
    }
}