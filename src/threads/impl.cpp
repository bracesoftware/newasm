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
    namespace threads
    {
        bool thread_now = false;
        std::string thread_decl = "\r";
        class object__
        {
            public:
            std::deque<std::string> contents;
            std::stringstream output;
        };
        std::unordered_map<std::string, newasm::threads::object__*> memory;
        std::vector<std::string> valid_threads;
        
        int thread_count = 0;
        namespace functions
        {
            void free_mem()
            {
                for(auto i = newasm::threads::memory.begin(); i != newasm::threads::memory.end(); ++i)
                {
                    if(i->second != nullptr)
                    {
                        delete i->second;
                        i->second = nullptr;
                    }
                }
            }
            int get_sysenter()
            {
                if(newasm::thread_line)
                {
                    return newasm::threads::sys_module;
                }
                return newasm::header::data::module;
            }
        }
    }
}