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
    namespace code_stream
    {
        int jumpto = 1;
        int jump = 0;
        const int invalid_lnidx = -1;
        namespace sections
        {
            const int config = 1;
            const int data = 2;
            const int start = 3;
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
        const int memory_leak = 16;
        const int invalid_config = 17;
        const int inline_proc = 18;
        const int unknown_fdx = 19;
        const int nested_redirect = 20;
        const int nested_csm = 21;
        const int uninptr_usage = 22;
        const int var_redef = 23;
        const int struct_redef = 24;
        const int unexpected_cbrace = 25;
        const int undefined_struct = 26;
        const int undefined_structmem = 27;

        const std::unordered_map<int, std::string> identifier = {
            {noterm_point, "NoTerminationPoint"},
            {invalid_section, "InvalidSection"},
            {invalid_proc, "InvalidProcedure"},
            {invalid_retn, "InvalidExitAttempt"},
            {sysreq_fail, "SystemRequirementsNotMet"},
            {stkhea_col, "StackHeapCollision"},
            {data_overflow, "DataOverflow"},
            {dtyp_mismatch, "DataTypeMismatch"},
            {label_redef, "LabelRedefinition"},
            {bus_err, "BusError"},
            {invalid_ins, "InvalidASMInstruction"},
            {mem_overflow, "ProgramMemoryOverflow"},
            {mem_underflow, "ProgramMemoryUnderflow"},
            {proc_redef, "ProcedureRedefinition"},
            {invalid_memacc, "InvalidMemoryAccess"},
            {invalid_syntax, "InvalidSyntax"},
            {memory_leak, "HeapMemoryLeak"},
            {invalid_config, "InvalidSysConfiguration"},
            {inline_proc, "InlineProcedure"},
            {unknown_fdx, "UnknownSystemCall"},
            {nested_redirect, "NestedExecFlowRedirection"},
            {nested_csm, "NestedCodeStreamModif"},
            {uninptr_usage, "UnassignedRefUsage"},
            {var_redef, "VariableRedefinition"},
            {struct_redef, "StructRedefinition"},
            {unexpected_cbrace, "UnexpectedClosingBrace"},
            {undefined_struct, "UndefinedStruct"},
            {undefined_structmem, "UndefinedStructMember"}
        };
    }
    namespace cmp_results
    {
        const int equal = 1;
        const int less = 2;
        const int greater = 3;
    }
    namespace datatypes
    {
        const int number = 1;
        const int decimal = 2;
        const int text = 3;
        const int reference = 4;
    }
    namespace constv
    {
        const std::string pxstr = "Program finished with exit code: ";
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
            const int max_mem_size = 512;
            int mem_size = 64;
        }
        std::string program_memory[newasm::mem::inf::max_mem_size];

        namespace regs
        {
            // non accessible registers
            int exc = 0; // cant be used lol
            int hea = 0; // heap size
            //mem registers
            int stk = newasm::mem::inf::max_mem_size - 1;
            int heaptr = 0; // pointer to heap adr
            //NORMAL REGISTERS
            int fdx = 0;
            std::string tlr = newasm::header::constants::inv_reg_val;
            std::string stl = newasm::header::constants::inv_reg_val;
            std::string psx = newasm::header::constants::inv_reg_val;
            std::string prp = newasm::header::constants::inv_reg_val;
            int cpr = 0;
            float cr0 = 0;
            float cr1 = 0;
            int br0 = 0;
            int br1 = 0;
            //int mcd = newasm::datatypes::decimal;
        }

        struct struct_member
        {
            int datatype;
            std::string name;
            std::string value;
        };

        std::unordered_map<std::string, std::string> data;
        std::unordered_map<std::string, int> datatypes;

        std::map<std::string, std::vector<std::string>> funcs;

        std::unordered_map<std::string, int> labels;

        std::unordered_map<std::string, std::vector<newasm::mem::struct_member>> structs;
        //std::vector<std::string> uninitialized_pointer;

        namespace functions
        {
            bool setup_memsize(int size)
            {
                if(size > newasm::mem::inf::max_mem_size)
                {
                    return false;
                }
                newasm::mem::inf::mem_size = size;
                newasm::mem::regs::hea = 0;
                newasm::mem::regs::heaptr = 0;
                newasm::mem::regs::stk = size-1;
                for(int i = 0; i < size; i++)
                {
                    newasm::mem::program_memory[i] = newasm::header::constants::nullstr;
                }
                return true;
            }
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
            bool islbln(int line)
            {
                for(std::unordered_map<std::string, int>::iterator i = labels.begin(); i != labels.end(); i++)
                {
                    if(i->second == line)
                    {
                        return true;
                    }
                }
                return false;
            }
        }
    }
}