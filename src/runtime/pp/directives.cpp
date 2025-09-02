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
    namespace pp
    {
        namespace impl
        {
            const std::string cfg_manip = "__newasm_cfg_manip";
            const std::string sys_int = "__newasm_sys_interrupt";
            void processDirectives(std::string directive, std::string arg)
            {
                directive = newasm::header::functions::trim(directive);
                arg = newasm::header::functions::trim(arg);

                if(directive == sys_int)
                {
                    if(arg == "__std_now__")
                    {
                        if(newasm::header::data::std_now__)
                        {
                            newasm::header::data::std_now__ = false;
                            return;
                        }
                        if(!newasm::header::data::std_now__)
                        {
                            newasm::header::data::std_now__ = true;
                            return;
                        }
                    }
                    return;
                }
                if(directive == cfg_manip)
                {
                    auto it = newasm::header::functions::split_fixed(arg, '=');
                    if(it.size() != 2)
                    {
                        newasm::header::functions::err("Invalid directive arguments.");
                        return;
                    }
                    it[0] = newasm::header::functions::trim(it[0]);
                    it[1] = newasm::header::functions::trim(it[1]);

                    if(it[0] == "__autobos")
                    {
                        if(it[1] == "__TRUE")
                        {
                            newasm::header::flags::autobos = true;
                            return;
                        }
                        if(it[1] == "__FALSE")
                        {
                            newasm::header::flags::autobos = false;
                            return;
                        }
                    }
                    return;
                }
                newasm::terminate(newasm::exit_codes::unknown_directive);
                return;
            }
        }
    }
}