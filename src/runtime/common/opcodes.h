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
    namespace opcodes
    {
        std::unordered_map<int, std::string> mem = {
            {1, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::load)}, //0x1
            {2, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::stor)}, //0x2
            {3, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::sysreq)}, //0x3
            {4, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::pop)}, //0x4
            {5, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::retn)}, //0x5
            {6, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::mov)}, //0x6
            {7, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::syscall)},
            {8, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::proc)},
            {9, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::call)},
            {10, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::rem)},
            {11, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::halt)},
            {12, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::push)},
            {13, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jmp)},
            {14, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::heap)},
            {15, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::cmp)},
            {16, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::je)},
            {17, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jne)},
            {18, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jl)},
            {19, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jg)},
            {20, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jle)},
            {21, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jge)},
            {22, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::ret)},
            {23, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::zero)},
            {24, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::inc)},
            {25, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::dec)},
            {26, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::exit)},
            {27, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::end)},
            {28, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::nop)},
            {29, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::add)},
            {30, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::sub)},
            {31, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::div)},
            {32, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::mul)},
            {33, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::exp)},
            {34, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::log)},
            {35, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::and__)},
            {36, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::or__)},
            {37, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::xor__)},
            {38, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::not__)},
            {39, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::shl)},
            {40, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::shr)},
            {41, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::db)},
            {42, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::malloc__)},
            {43, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::free__)},
            {44, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::stack)},
            {45, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::wait)},
            {46, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::sysenter)},
            {47, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::int__)},
            {48, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::cls)},
            {49, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::vmov)},
            {50, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::switch__)},
            {51, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::case__)},
            {52, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::default__)},
            {53, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::in__)},
            {54, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::out__)},
            {55, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::cast__)},
            {56, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::async__)},
            {57, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::lea)},
            {58, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::thread__)},
            {59, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::send)},
            {60, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::recv)},

            {100, newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::__say)}
        };
    }
}
