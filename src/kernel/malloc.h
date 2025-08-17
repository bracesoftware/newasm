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
    namespace malloc
    {
        std::vector<int> meta;
        //int memory[10 * 1024 * 1024];
        std::unordered_map<int, int> types;
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
            return;
        }
    }
}
#endif