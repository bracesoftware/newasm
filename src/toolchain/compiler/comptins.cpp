// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(pp, {
    //setup goes here
});

// little preprocessor

namespace newasm
{
    namespace compiler
    {
        namespace meta
        {
            std::unordered_map<std::string, std::string> defines;
        }

        inline unsigned int iscomptins(std::string ins)
        {
            auto it = newasm::compiler::instructions.find(ins);
            if(it != newasm::compiler::instructions.end())
            {
                return it->second;
            }
            return 0;
        }

        inline std::string parse_def(std::string suf)
        {
            std::string tobeparsed = newasm::header::functions::trim(suf);
            auto it = newasm::compiler::meta::defines.find(tobeparsed);
            if(it != newasm::compiler::meta::defines.end())
            {
                tobeparsed = newasm::compiler::meta::defines.at(tobeparsed);
            }
            return tobeparsed;
        }
        inline void process_compti(std::string ins)
        {
            ins = newasm::header::functions::trim(ins);

            auto it = newasm::compiler::instructions.find(ins);
            if(it == newasm::compiler::instructions.end())
            {
                return;
            }
            switch(it->second)
            {
                case newasm::compiler::fi__:
                {
                    newasm::compiler::data::IfResult = true;
                    return;
                }
            }
            return;
        }
        inline void process_comptis(std::string ins, std::string arg1)
        {
            ins = newasm::header::functions::trim(ins);
            arg1 = newasm::header::functions::trim(arg1);

            auto it = newasm::compiler::instructions.find(ins);
            if(it == newasm::compiler::instructions.end())
            {
                return;
            }
            switch(it->second)
            {
                case newasm::compiler::ifdef__:
                {
                    auto p = newasm::compiler::meta::defines.find(arg1);
                    if(p == newasm::compiler::meta::defines.end())
                    {
                        newasm::compiler::data::IfResult = false;
                        return;
                    }
                    newasm::compiler::data::IfResult = true;
                    return;
                }
                case newasm::compiler::ifndef__:
                {
                    auto p = newasm::compiler::meta::defines.find(arg1);
                    if(p == newasm::compiler::meta::defines.end())
                    {
                        newasm::compiler::data::IfResult = true;
                        return;
                    }
                    newasm::compiler::data::IfResult = false;
                    return;
                }
                case newasm::compiler::pragma__:
                {
                    if(arg1 == "errtest")
                    {
                        newasm::header::functions::err("Test error pragma detected.");
                        return;
                    }
                    return;
                }
                case newasm::compiler::undef__:
                {
                    auto p = newasm::compiler::meta::defines.find(arg1);
                    if(p == newasm::compiler::meta::defines.end())
                    {
                        newasm::compiler::abort(newasm::compiler::fail::invalid_symbol);
                        return;
                    }
                    
                    newasm::compiler::meta::defines.erase(arg1);
                    return;
                }
                case newasm::compiler::using__:
                {
                    auto arg = arg1;
                    if(!newasm::header::functions::istext(arg))
                    {
                        newasm::compiler::abort(newasm::compiler::fail::expected_token);
                        return;
                    }
                    arg = newasm::header::functions::remq(arg);
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
                    {
                        newasm::kernel::cfg::IOStream = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
                    {
                        newasm::kernel::cfg::FileStream = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ext))
                    {
                        newasm::kernel::cfg::Extensions = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
                    {
                        newasm::kernel::cfg::ContainerManipulation = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
                    {
                        newasm::kernel::cfg::TextOperations = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
                    {
                        newasm::kernel::cfg::Network = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
                    {
                        newasm::kernel::cfg::Memory = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
                    {
                        newasm::kernel::cfg::Chrono = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
                    {
                        newasm::kernel::cfg::Thread = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::tuple))
                    {
                        newasm::kernel::cfg::Tuple = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::tcp))
                    {
                        newasm::kernel::cfg::TCProtocol = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::http))
                    {
                        newasm::kernel::cfg::HTTP = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::math))
                    {
                        newasm::kernel::cfg::Math = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::misc))
                    {
                        newasm::kernel::cfg::Misc = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::crypto))
                    {
                        newasm::kernel::cfg::Crypto = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ctx))
                    {
                        newasm::kernel::cfg::Context = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs_vdsk))
                    {
                        newasm::kernel::cfg::FileStream_VDSK = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cfg))
                    {
                        newasm::kernel::cfg::Configuration = true;
                        return;
                    }
                    newasm::compiler::abort(newasm::compiler::fail::invalid_krnlmod);
                    return;
                }
            }
        }
        void process_comptiso(std::string ins, std::string arg1, std::string arg2)
        {
            ins = newasm::header::functions::trim(ins);
            arg1 = newasm::header::functions::trim(arg1);
            arg2 = newasm::header::functions::trim(arg2);

            auto it = newasm::compiler::instructions.find(ins);
            if(it == newasm::compiler::instructions.end())
            {
                return;
            }
            switch(it->second)
            {
                case newasm::compiler::def:
                {
                    auto p = newasm::compiler::meta::defines.find(arg1);
                    if(p != newasm::compiler::meta::defines.end())
                    {
                        newasm::compiler::abort(newasm::compiler::fail::constant_redef);
                        return;
                    }

                    newasm::compiler::meta::defines[arg1] = arg2;
                    return;
                }
            }
            return;
        }
    }
}