// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(KRNL_____drivers, {
    //setup goes here
});

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    bool thread_line = false;
    namespace threads
    {
        std::string now;
        unsigned int id_now = 0;
        std::vector<int> sys_module;
    }
}