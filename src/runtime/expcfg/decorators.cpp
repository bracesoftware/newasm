// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace expcfg
    {
        //decorator settings
        bool lockbool = false;
        bool volatilebool = false;
        bool transientbool = false;

        //impl
        inline void process_dec(newasm::compiler::lineData& line)
        {
            switch(line.letsDecorateVariables)
            {
                case INVALID_INS:
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return;
                }
                case newasm::kernel::makeHash(newasm::decorators::DESTRUCTIVE, newasm::decorators::id::LOCK):
                {
                    newasm::expcfg::lockbool = false;
                    return;
                }
                case newasm::kernel::makeHash(newasm::decorators::DESTRUCTIVE, newasm::decorators::id::VOLATILE):
                {
                    newasm::expcfg::volatilebool = false;
                    return;
                }
                case newasm::kernel::makeHash(newasm::decorators::DESTRUCTIVE, newasm::decorators::id::TRANSIENT):
                {
                    newasm::expcfg::transientbool = false;
                    return;
                }
                case newasm::kernel::makeHash(newasm::decorators::CONSTRUCTIVE, newasm::decorators::id::LOCK):
                {
                    newasm::expcfg::lockbool = true;
                    return;
                }
                case newasm::kernel::makeHash(newasm::decorators::CONSTRUCTIVE, newasm::decorators::id::VOLATILE):
                {
                    newasm::expcfg::volatilebool = true;
                    return;
                }
                case newasm::kernel::makeHash(newasm::decorators::CONSTRUCTIVE, newasm::decorators::id::TRANSIENT):
                {
                    newasm::expcfg::transientbool = true;
                    return;
                }
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            //newasm::progwin::api::cout("Deco::"+text);
            #if 0
            /*
            DESTRUCTIVE DECORATORS
            */
            if(text.at(0) == '!')
            {
                if(newasm::header::functions::trim(text.substr(1)) == newasm::expcfg::decorators.at(newasm::expcfg::lock))
                {
                    if(newasm::expcfg::lockbool)
                    {
                        newasm::expcfg::lockbool = false;
                        return;
                    }
                }
                if(newasm::header::functions::trim(text.substr(1)) == newasm::expcfg::decorators.at(newasm::expcfg::volatile__))
                {
                    if(newasm::expcfg::volatilebool)
                    {
                        newasm::expcfg::volatilebool = false;
                        return;
                    }
                }
                if(newasm::header::functions::trim(text.substr(1)) == newasm::expcfg::decorators.at(newasm::expcfg::transient__))
                {
                    if(newasm::expcfg::transientbool)
                    {
                        newasm::expcfg::transientbool = false;
                        return;
                    }
                }
            }
            /*
            CONSTRUCTIVE DECORATORS
            */
            if(text == newasm::expcfg::decorators.at(newasm::expcfg::lock))
            {
                newasm::expcfg::lockbool = true;
                return;
            }
            if(text == newasm::expcfg::decorators.at(newasm::expcfg::volatile__))
            {
                newasm::expcfg::volatilebool = true;
                return;
            }
            if(text == newasm::expcfg::decorators.at(newasm::expcfg::transient__))
            {
                newasm::expcfg::transientbool = true;
                return;
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            #endif
            return;
        }
    }
}