// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    namespace threads
    {
        bool thread_now = false;
        std::string thread_decl = "?";
        class object__
        {
            private:
            bool prepared = false;
            public:
            std::vector<newasm::compiler::lineData> contents;
            std::stringstream output;
            std::string returned_val;
            bool returned = false;
            bool paused = false;
            std::unordered_map<std::string, int> labels;

            int lcx = 0;

            explicit inline object__()
            {
                #if 0
                this->contents.reserve(500);
                #endif
            }

            inline void recompile_threadProc()
            {
                if(this->prepared)
                {
                    return;
                }
                this->prepared = true;

                if(this->contents.empty())
                {
                    return;
                }

                for(int i = 0; i < this->contents.size(); ++i)
                {
                    if(this->contents.at(i).type == newasm::compiler::sealedLabel)
                    {
                        //std::cout << "Successfully added label: `" << this->contents.at(i).other << "`" << std::endl;
                        this->labels[this->contents.at(i).other] = i;
                        this->contents.at(i).type = newasm::compiler::empty;
                        continue;
                    }
                }

                return;
            }
        };
        std::unordered_map<std::string, newasm::threads::object__*> memory;
        std::vector<std::string> valid_threads;
        
        int thread_count = 0;
        namespace functions
        {
            void free_mem()
            {
                valid_threads.clear();
                if(newasm::threads::memory.empty())
                {
                    return;
                }
                for(auto i = newasm::threads::memory.begin(); i != newasm::threads::memory.end(); ++i)
                {
                    if(i->second != nullptr)
                    {
                        delete i->second;
                    }
                }
                newasm::threads::memory.clear();
                return;
            }
            int get_sysenter()
            {
                if(newasm::thread_line)
                {
                    return newasm::threads::sys_module.at(newasm::threads::now);
                }
                return newasm::header::data::module;
            }
        }
    }
}