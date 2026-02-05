/*

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/

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