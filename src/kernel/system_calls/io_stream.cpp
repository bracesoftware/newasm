// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif


namespace newasm
{
    namespace syscalls
    {
        namespace iostream
        {
            inline void out_bopr(const std::string &reg)
            {
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ext))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
                {
                    std::cout << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
                {
                    std::cout << "";
                }
                if(reg == "\%nl")
                {
                    std::cout << "";
                }
                if(reg == "&\%null")
                {
                    std::cout << "null";
                }
                /*if(reg == "\%endl")
                {
                    std::cout << "\n";
                }*/
                if(reg == newasm::header::constants::inv_reg_val)
                {
                    std::cout << "";
                }
                else
                {
                    std::cout << reg;
                }
            }
            inline std::string get_ref_val__2(const std::string &reg)
            {
                std::stringstream ss;
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ext))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
                {
                    ss << "";
                }
                if(reg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
                {
                    ss << "";
                }
                if(reg == "\%nl")
                {
                    ss << "";
                }
                if(reg == "&\%null")
                {
                    ss << "null";
                }
                /*if(reg == "\%endl")
                {
                    ss << "\n";
                }*/
                if(reg == newasm::header::constants::inv_reg_val)
                {
                    ss << "<?>";
                }
                else
                {
                    ss << reg;
                }
                return ss.str();
            }
        }
    }
}