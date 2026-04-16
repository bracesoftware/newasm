// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(jit_compiler, {
    //setup goes here
});

namespace newasm
{
    namespace native_jit
    {
        void print(const std::string& text)
        {
            newasm::jit_print(text.data());
            return;
        }
    }
}