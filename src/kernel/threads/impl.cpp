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
        class object__
        {
            private bool prepared = false;
            public std::vector<newasm::compiler::lineData> contents;
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
                this->labels.max_load_factor(MAX_LOAD_FACTOR);

                for(int i = 0; i < this->contents.size(); ++i)
                {
                    if(this->contents.at(i).type == newasm::compiler::sealedLabel)
                    {
                        //std::cout << "Successfully added label: `" << this->contents.at(i).other << "`" << std::endl;
                        auto k = this->contents.at(i).other;
                        if(this->labels.find(k) != this->labels.end())
                        {
                            newasm::terminate(newasm::exit_codes::label_redef);
                        }
                        this->labels[k] = i;
                        newasm::sealedLabels->push_back(k);
                        this->contents.at(i).type = newasm::compiler::empty;
                        continue;
                    }
                }
                for(int i = 0; i < this->contents.size(); ++i)
                {
                    auto& bytecode = this->contents.at(i);
                    if(
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jmp or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jz or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jnz or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::je or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jne or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jl or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jle or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jg or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jge or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::callc
                    )
                    {
                        if(bytecode.tokens.size() != 2)
                        {
                            continue;
                        }
                        auto& label_name = bytecode.tokens[1];

                        if(this->labels.find(label_name) == this->labels.end())
                        {
                            newasm::terminate(newasm::exit_codes::bus_err);
                            break;
                        }
                        bytecode.jumpinTo = this->labels.at(label_name);

                        if(bytecode.whatAmIDoing == newasm::core::lang_inf::callc)
                        {
                            bytecode.returninTo = i;
                        }
                        continue;
                    }
                    if(
                        bytecode.whatAmIDoing == newasm::core::lang_inf::loop
                    )
                    {
                        if(bytecode.tokens.size() != 3)
                        {
                            continue;
                        }
                        auto& label_name = bytecode.tokens[2];
                        if(this->labels.find(label_name) == this->labels.end())
                        {
                            newasm::terminate(newasm::exit_codes::bus_err);
                            break;
                        }
                        bytecode.jumpinTo = this->labels.at(label_name);
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
                #if 0
                if(newasm::threads::memory.empty())
                {
                    return;
                }
                #endif
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