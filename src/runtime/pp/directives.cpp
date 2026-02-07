// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace pp
    {
        namespace config
        {
            bool autobos_old = false;

            bool krnl_iostream_old = false;
        }
        namespace impl
        {
            const std::string cfg_manip = "__newasm_cfg_manip";
            const std::string sys_int = "__newasm_sys_interrupt";
            const std::string kernel_config = "__newasm_krnl_config";

            void processDirectives(std::string directive, std::string arg)
            {
                directive = newasm::header::functions::trim(directive);
                arg = newasm::header::functions::trim(arg);

                if(directive == sys_int)
                {
                    if(arg == "__std_now__")
                    {
                        if(newasm::header::data::std_now__)
                        {
                            newasm::header::data::std_now__ = false;
                            return;
                        }
                        if(!newasm::header::data::std_now__)
                        {
                            newasm::header::data::std_now__ = true;
                            return;
                        }
                    }
                    return;
                }
                if(directive == kernel_config)
                {
                    auto vec = newasm::header::functions::split_fixed(arg, '/');

                    vec[0] = newasm::header::functions::trim(vec[0]);
                    vec[1] = newasm::header::functions::trim(vec[1]);

                    if(vec[0] == "IOS")
                    {
                        if(vec[1] == "YES__")
                        {
                            __newasm_rem(newasm::header::functions::krnl("IOS enabled."););

                            newasm::pp::config::krnl_iostream_old = newasm::kernel::cfg::IOStream;
                            newasm::kernel::cfg::IOStream = true;
                            return;
                        }
                        if(vec[1] == "NO__")
                        {
                            __newasm_rem(newasm::header::functions::krnl("IOS disabled."););
                            
                            newasm::kernel::cfg::IOStream = newasm::pp::config::krnl_iostream_old;
                            return;
                        }
                    }
                    return;
                }
                if(directive == cfg_manip)
                {
                    auto it = newasm::header::functions::split_fixed(arg, '=');
                    if(it.size() != 2)
                    {
                        newasm::header::functions::err("Invalid directive arguments.");
                        return;
                    }
                    it[0] = newasm::header::functions::trim(it[0]);
                    it[1] = newasm::header::functions::trim(it[1]);

                    if(it[0] == "__autobos")
                    {
                        if(it[1] == "__TRUE")
                        {
                            newasm::pp::config::autobos_old = newasm::header::flags::autobos; // to keep the int 0x3 status
                            newasm::header::flags::autobos = true;
                            return;
                        }
                        if(it[1] == "__FALSE")
                        {
                            newasm::header::flags::autobos = newasm::pp::config::autobos_old;
                            return;
                        }
                    }
                    return;
                }
                newasm::terminate(newasm::exit_codes::unknown_directive);
                return;
            }
        }
    }
}