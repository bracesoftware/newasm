// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(KRNL_THREADS, {
    //setup goes here
});

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    namespace threads
    {
        bool thread_now = false;
        std::string thread_decl = "?";

        //std::unordered_map<std::string, newasm::threads::object__*> memory;
        //std::vector<std::string> valid_threads;
        
        int thread_count = 0;
        namespace functions
        {
            int get_sysenter()
            {
                if(newasm::thread_line)
                {
                    return newasm::threads::sys_module[newasm::threads::id_now];
                }
                return newasm::header::data::module;
            }
        }
    }
}