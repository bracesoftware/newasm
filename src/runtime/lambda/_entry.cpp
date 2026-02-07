// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace lambda
    {
        class _obj
        {
            public:
            std::string line; //xd
            std::vector<std::string> contents; //contents of a lambda proc
            std::string result; // returned value
            bool ret = false;
            bool thread = false;
        };
        newasm::lambda::_obj GLOBAL; // crazy 
        bool lambda_now = false; // if lambda is being declared
        bool process = false; // if lambda contents is being processed
    }
}