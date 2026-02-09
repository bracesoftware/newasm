// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace lambda
    {
        bool lambda_now = false; // if lambda is being declared
        
        class _obj final
        {
            public:
            std::string line; //xd
            std::vector<std::string> contents; //contents of a lambda proc
            std::string result; // returned value
            bool ret = false;
            bool thread = false;

            inline void reset_forKernel()
            {
                this->line.clear();
                this->contents.clear();
                this->result.clear();
                this->ret = false;
                this->thread = false;
                newasm::lambda::lambda_now = false;
                newasm::lambda::process = false;
                return;
            }
        };
        newasm::lambda::_obj GLOBAL; // crazy 
    }
}