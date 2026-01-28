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
    namespace garbage
    {
        int FLAG = 0;
        std::vector<std::string> addr__;
    }
    class garbageCollector
    {
        public:
        inline static void DO()
        {
            if(newasm::garbage::FLAG == 0)
            {
                return;
            }

            newasm::garbage::FLAG = 0;
            int addr;

            for(int i = 0; i < newasm::garbage::addr__.size(); ++i)
            {
                auto& it = newasm::variables::ids.at(newasm::garbage::addr__.at(i));
                if(it.type = newasm::datatypes::number)
                {
                    addr = it.addr;
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(int));
                    continue;
                }
                if(it.type = newasm::datatypes::decimal)
                {
                    addr = it.addr;
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(float));
                    continue;
                }
                if(it.type = newasm::datatypes::character)
                {
                    addr = it.addr;
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                    continue;
                }
                if(it.type = newasm::datatypes::text)
                {
                    addr = it.addr;
                    int buffer_len;
                    std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                    continue;
                }
            }

            newasm::garbage::addr__.clear();
            return;
        }
    };
}