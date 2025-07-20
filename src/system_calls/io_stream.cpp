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
    namespace syscalls
    {
        namespace iostream
        {
            void out_bopr(const std::string &reg)
            {
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::exf))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
                {
                    std::cout << "";
                }
                if(reg == "\%nl")
                {
                    std::cout << "";
                }
                if(reg == "&\%null")
                {
                    std::cout << "null";
                }
                if(reg == "\%endl")
                {
                    std::cout << "\n";
                }
                if(reg == newasm::header::constants::inv_reg_val)
                {
                    std::cout << "";
                }
            }
            std::string get_ref_val__2(const std::string &reg)
            {
                std::stringstream ss;
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::exf))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
                {
                    ss << "";
                }
                if(reg == "\%nl")
                {
                    ss << "";
                }
                if(reg == "&\%null")
                {
                    ss << "null";
                }
                if(reg == "\%endl")
                {
                    ss << "\n";
                }
                if(reg == newasm::header::constants::inv_reg_val)
                {
                    ss << "<?>";
                }
                return ss.str();
            }
        }
    }
}