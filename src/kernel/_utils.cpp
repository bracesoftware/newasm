// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


namespace newasm
{
    namespace header
    {
        namespace functions
        {
            void krnl(std::string text)
            {
                std::cout << newasm::header::col::kernel << "kernel note: `" << newasm::header::col::light_red
                << newasm::kernel::syscalls.at({newasm::threads::functions::get_sysenter(),newasm::mem::regs::fdx.get_value()})
                << newasm::header::col::kernel << "`: " << newasm::header::col::gray << text << newasm::header::col::reset << std::endl;
            }
        }
    }
}