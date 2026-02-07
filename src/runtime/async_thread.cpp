// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    namespace async_thread
    {
        std::atomic<bool> running = true;
        void entry()
        {
            if constexpr(0)
            {
                while(newasm::async_thread::running)
                {
                    //shitness
                }
            }
            return;
        }
    }
}