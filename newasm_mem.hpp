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

Portions created by the Initial Developer are Copyright (c) 2022
the Initial Developer. All Rights Reserved.

*/

namespace newasm
{
    namespace code_stream
    {
        int jumpto = 1;
        int jump = 0;
        const int invalid_lnidx = -1;
        namespace sections
        {
            const int data = 1;
            const int start = 2;
        }
    }
    namespace exit_codes
    {
        const int noterm_point = 0;
        const int invalid_section = 1;
        const int invalid_proc = 2;
        const int invalid_retn = 3;
        const int sysreq_fail = 4;
        const int stkhea_col = 5;
        const int data_overflow = 6;
        const int dtyp_mismatch = 7;
        const int label_redef = 8;
        const int bus_err = 9;
        const int invalid_ins = 10;
        const int mem_overflow = 11;
        const int mem_underflow = 12;
        const int proc_redef = 13;
        const int invalid_memacc = 14;
        const int invalid_syntax = 15;
    }
    namespace datatypes
    {
        const int number = 1;
        const int decimal = 2;
        const int text = 3;
    }
    namespace constv
    {
        const std::string pxstr = "Program finished with exit code : ";
    }
    namespace system
    {
        bool terminated = false;
        int section = 0;
        int stop = 0;
        int stoproc = 0;
        std::string cproc;
    }
    namespace mem
    {
        namespace inf
        {
            const int mem_size = 100;
        }
        std::string program_memory[newasm::mem::inf::mem_size];

        namespace regs
        {
            // non accessible registers
            int hea = 0;
            //mem registers
            int stk = newasm::mem::inf::mem_size - 1;
            int heaptr = 0;
            //NORMAL REGISTERS
            int exc = 0;
            int fdx = 0;
            std::string tlr = newasm::header::constants::inv_reg_val;
            std::string stl = newasm::header::constants::inv_reg_val;
            std::string psx = newasm::header::constants::inv_reg_val;
        }

        std::unordered_map<std::string, std::string> data;
        std::unordered_map<std::string, int> datatypes;
        std::unordered_map<std::string, std::vector<std::string>> funcs;
        std::unordered_map<std::string, int> labels;

        namespace functions
        {
            bool check_stkhea_col()
            {
                return (!(newasm::mem::regs::hea < newasm::mem::regs::stk));
            }
            template<typename T> bool datavalid(std::string dataname, T &dat)
            {
                for(auto it = dat.begin(); it != dat.end(); it++)
                {
                    if(it->first == dataname)
                    {
                        return true;
                    }
                }
                return false;
            }
            void out_bopr(std::string reg)
            {
                if(reg == "\%ios")
                {
                    std::cout << "";
                }
                if(reg == "\%nl")
                {
                    std::cout << "";
                }
                if(reg == "\%endl")
                {
                    std::cout << newasm::header::constants::endl;
                }
                if(reg == newasm::header::constants::inv_reg_val)
                {
                    std::cout << "";
                }
            }
        }
    }
}