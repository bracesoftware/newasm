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
    namespace core
    {
        namespace lang_inf
        {
            const int load = 1;
            const int stor = 2;
            const int sysreq = 3;
            const int pop = 4;
            const int retn = 5;
            const int mov = 6;
            const int syscall = 7;
            const int proc = 8;
            const int call = 9;
            const int rem = 10;
            const int halt = 11;
            const int push = 12;
            const int jmp = 13;
            const int heap = 14;
            const int cmp = 15;
            const int je = 16;
            const int jne = 17;
            const int jl = 18;
            const int jg = 19;
            const int jle = 20;
            const int jge = 21;

            const int ret = 22;
            const int zero = 23;
            const int inc = 24;
            const int dec = 25;

            const int exit = 26;
            const int end = 27;
            const int nop = 28;
            const int add = 29;
            const int sub = 30;
            const int div = 31;
            const int mul = 32;
            const int exp = 33;
            const int log = 34;
            const int and__ = 35;
            const int or__ = 36;
            const int xor__ = 37;
            const int not__ = 38;
            const int shl = 39;
            const int shr = 40;

            const std::unordered_map<int, std::string> instruction_set = {
                {load,"load"},
                {stor,"stor"},
                {sysreq, "sysreq"},
                {pop, "pop"},
                {retn, "retn"},
                {mov, "mov"},
                {syscall, "syscall"},
                {proc, "proc"},
                {call, "call"},
                {rem, "rem"},
                {halt, "halt"},
                {push, "push"},
                {jmp, "jmp"},
                {heap, "heap"},
                {cmp, "cmp"},
                {je, "je"},
                {jne, "jne"},
                {jl, "jl"},
                {jg, "jg"},
                {jle, "jle"},
                {jge, "jge"},

                {ret, "ret"},
                {zero, "zero"},
                {inc, "inc"},
                {dec, "dec"},

                {exit, "exit"},
                {end, "end"},
                {nop, "nop"},
                {add, "add"},
                {sub, "sub"},
                {div, "div"},
                {mul, "mul"},
                {exp, "exp"},
                {log, "log"},
                {and__, "and"},
                {or__, "or"},
                {xor__, "xor"},
                {not__, "not"},
                {shl, "shl"},
                {shr, "shr"}
            };

            namespace utils
            {
                bool iskeyword(const std::string &str)
                {
                    for(auto i = newasm::core::lang_inf::instruction_set.begin(); i != newasm::core::lang_inf::instruction_set.end(); i++)
                    {
                        if(i->second == str)
                        {
                            return true;
                        }
                    }
                    return false;
                }
            }
        }
    }
}