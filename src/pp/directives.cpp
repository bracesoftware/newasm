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
            void processDirectives(std::string directive, std::string arg)
            {
                if(directive == newasm::core::lang_inf::directives::identifiers__.at(newasm::core::lang_inf::directives::using__))
                {
                    if(!newasm::header::functions::istext(arg))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return;
                    }
                    arg = newasm::header::functions::remq(arg);
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
                    {
                        newasm::kernel::cfg::IOStream = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
                    {
                        newasm::kernel::cfg::FileStream = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ext))
                    {
                        newasm::kernel::cfg::Extensions = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
                    {
                        newasm::kernel::cfg::ContainerManipulation = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
                    {
                        newasm::kernel::cfg::TextOperations = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
                    {
                        newasm::kernel::cfg::Network = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
                    {
                        newasm::kernel::cfg::Memory = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
                    {
                        newasm::kernel::cfg::Chrono = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
                    {
                        newasm::kernel::cfg::Thread = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::tuple))
                    {
                        newasm::kernel::cfg::Tuple = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::tcp))
                    {
                        newasm::kernel::cfg::TCProtocol = true;
                        return;
                    }
                    newasm::terminate(newasm::exit_codes::invalid_config);
                    return;
                }
                newasm::terminate(newasm::exit_codes::unknown_directive);
                return;
            }
        }
    }
}