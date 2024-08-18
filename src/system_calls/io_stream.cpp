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
                if(reg == "\%exf")
                {
                    std::cout << "";
                }
                if(reg == "\%ios")
                {
                    std::cout << "";
                }
                if(reg == "\%fs")
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
        }
    }
}