// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif


namespace newasm
{
    namespace syscalls
    {
        namespace execflow
        {
            void main()
            {
                return;
            }
        }
    }
    namespace header
    {
        namespace execution_flow
        {
            int entry_start_line = 1;
            std::string file = "";
            bool exec_redirected = false;
            std::string entry_exec = "";
        }
    }
}