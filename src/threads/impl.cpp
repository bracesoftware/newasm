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
            //std::vector<std::string> contents_old;
            //int lastlndx = 0;
            //int size = 0;
        };
        std::unordered_map<std::string, newasm::threads::object__*> memory;
        std::vector<std::string> valid_threads;
        //std::vector<std::string> finished_threads;
        int thread_count = 0;
        namespace functions
        {
            #ifdef NEWASM_MEGA_ERROR
            bool finished(std::string thread_name)
            {
                if(newasm::threads::memory.at(thread_name)->lastlndx == newasm::threads::memory.at(thread_name)->size)
                {
                    return true;
                }
                return false;
            }
            std::string getlastline(std::string thread_name)
            {
                int line = 0;
                for(auto i = newasm::threads::memory.at(thread_name)->contents.begin(); i < newasm::threads::memory.at(thread_name)->contents.end(); i++)
                {
                    if(line == newasm::threads::memory.at(thread_name)->lastlndx)
                    {
                        newasm::threads::memory.at(thread_name)->lastlndx++;
                        return *i;
                    }
                    line++;
                }
                return static_cast<std::string>("__rip_thread__");
            }
            #endif
            void free_mem()
            {
                for(auto i = newasm::threads::memory.begin(); i != newasm::threads::memory.end(); i++)
                {
                    delete i->second;
                }
            }
            /*void proceed()
            {
                
            }*/
        }
    }
}