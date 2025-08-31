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
        namespace constants
        {
            const std::string data_folder = ".newasm";
            const std::string temp_vers = "__temp_vers.txt";
            const std::string virtual_mem = "vmem._sys";
            const std::string dynlib_ex = ".newasm_dl";
            const std::string setup_ex = ".newasm_set";
            const std::string progwin = 
            #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
                "progwin_newasm.exe"
            #elif _NEWASM_OS == _NEWASM_OS_linux || _NEWASM_OS == _NEWASM_OS_android
                "progwin_newasm.a"
            #endif
            ;
            const std::string cache_folder = "_cache";
            const std::string ipc_file = "ipc._sys";
            const std::string disk_file = "disk.bin";

            const std::string user_folder = "user";

            #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
                const std::string separator = "\\";
            #elif _NEWASM_OS == _NEWASM_OS_linux || _NEWASM_OS == _NEWASM_OS_android
                const std::string separator = "/";
            #endif
        }
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

            const int db = 41;
            const int malloc__ = 42;
            const int free__ = 43;
            const int stack = 44;
            const int wait = 45;
            const int sysenter = 46;
            const int xchg = 47;
            const int int__ = 48;
            const int cls = 49;

            const int await__ = 50;
            const int retf = 51;
            const int vmov = 52;

            const int switch__ = 53;
            const int case__ = 54;
            const int default__ = 55;

            const int in__ = 56;
            const int out__ = 57;

            const int cast__ = 58;
            const int async__ = 59;

            const int lea = 60;
            const int thread__ = 61;

            const int send = 62;
            const int recv = 63;

            const int del = 64;
            const int movaddr = 65;

            const int jz = 66;
            const int jnz = 67;

            const int movasx = 68;

            const int __say = 100;

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
                {shr, "shr"},

                {db, "db"},
                {malloc__, "malloc"},
                {free__, "free"},
                {stack, "stack"},
                {wait, "wait"},
                {sysenter, "sysenter"},
                {xchg, "xchg"},
                {int__, "int"},
                {cls, "cls"},

                {await__, "await"},
                {retf, "retf"},
                {vmov, "vmov"},

                {switch__, "switch"},
                {case__, "case"},
                {default__, "default"},

                {in__, "in"},
                {out__, "out"},

                {cast__, "cast"},
                {async__, "async"},
                {lea, "lea"},
                {thread__, "thread"},

                {send, "send"},
                {recv, "recv"},

                {del, "del"},
                {movaddr, "movaddr"},

                {jz, "jz"},
                {jnz, "jnz"},

                {movasx, "movasx"}, //movaddr for tuples

                {__say, "__say"}
            };

            namespace refs
            {
                const int ios = 1;
                const int ext = 2;
                const int cmanip = 3;
                const int fs = 4;
                const int txtop = 5;
                const int net = 6;
                const int mem = 7;
                const int chrono = 8;
                const int thread = 9;
                const int tuple = 10;
                const int tcp = 11;
                const int http = 12;

                const int math = 13;

                const std::unordered_map<int, std::string> identifiers__ = {
                    {ios, "ios"},
                    {ext, "ext"},
                    {cmanip, "cmanip"},
                    {fs, "fs"},
                    {txtop, "txtop"},
                    {net, "net"},
                    {mem, "mem"},
                    {chrono, "chrono"},
                    {thread, "thread"},
                    {tuple, "tuple"},
                    {tcp, "tcp"},
                    {http, "http"},
                    {math, "math"}
                };
            }
            namespace directives
            {
                const int using__ = 1;

                const std::unordered_map<int, std::string> identifiers__ = {
                    {using__, "using"}
                };
            }

            namespace typenames
            {
                const int num = 1;
                const int decm = 2;
                const int txt = 3;
                const int char__ = 4;
                const int ref = 5;
                const int obj = 6;
                const int cont = 7;
				const int tuple = 8;
				
                const int class__ = 9;

                const int bit_arr = 10;
                const int bin_tree = 11;
                const int chan = 12;

                const std::unordered_map<int, std::string> identifiers__ = {
                    {num, "intg"},
                    {decm, "float"},
                    {txt, "string"},
                    {char__, "char"},
                    {ref, "ref"},
                    {obj, "obj"},
                    {cont, "cont"},
					{tuple, "tuple"},

                    {class__, "class"},
					
                    {bit_arr, "bit_arr"},
                    {bin_tree, "bin_tree"},
                    {chan, "chan"}
                };
            }

            namespace cmds
            {
                const int exit__ = 1;
                const int help__ = 2;
                const int install__ = 3;
                const int repl__ = 4;
                const int login__ = 5;
                const int logout__ = 6;
                const int addenv__ = 7;
                const int remenv__ = 8;
                const int modenv__ = 9;
                const int printenv__ = 10;

                const std::unordered_map<int, std::string> identifiers__ = {
                    {exit__, "exit"},
                    {help__, "help"},
                    {repl__, "repl"},
                    {install__, "install"},
                    {login__, "login"},
                    {logout__, "logout"},
                    {addenv__, "addenv"},
                    {remenv__, "remenv"},
                    {modenv__, "modenv"},
                    {printenv__, "printenv"}
                };
            }

            namespace events
            {
                const int exit = 1;

                const std::unordered_map<int, std::string> identifiers__ = {
                    {exit, "~exit"}
                };
            }

            namespace utils
            {
                bool iskeyword(const std::string &str)
                {
                    for(auto i = newasm::core::lang_inf::instruction_set.begin(); i != newasm::core::lang_inf::instruction_set.end(); ++i)
                    {
                        if(i->second == str)
                        {
                            return true;
                        }
                    }
                    return false;
                }

                const int instance = 0;

                bool iskeyword(const std::string& str, int what)
                {
                    if(what == instance)
                    {
                        if(str == "instance")
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