// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(KRNLMALLOC, {
    //setup goes here
});

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    namespace malloc
    {
        std::vector<int> meta;
        //int memory[10 * 1024 * 1024];
        newasm::_std::linear_map<int, int> types;
    }
    namespace stack
    {
        std::unordered_map<std::string, std::string> events = {
            {"test","test2"}
        };
        class macro_data
        {
            public:
            std::vector<std::string> contents;
        };
        std::unordered_map<std::string, newasm::stack::macro_data*> macros;

        void free_macro_mem()
        {
            if(macros.empty())
            {
                return;
            }
            for(std::unordered_map<std::string, newasm::stack::macro_data*>::iterator i = newasm::stack::macros.begin();
            i != newasm::stack::macros.end(); ++i)
            {
                if(i->second != nullptr) delete i->second;
            }
            newasm::stack::macros.clear();
            return;
        }
    }
}

#if 0
namespace newasm
{
    namespace mem
    {
        class malloc_info
        {
            public:
                int size = 0;
                int heapsize_new = 0;
        };
    }
}

namespace newasm
{
    newasm::mem::malloc_info *allocation_data = nullptr;
}
#endif