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
        const int lock = 1;

        std::unordered_map<int, std::string> decorators = {
            {lock, "lock"}
        };

        //decorator settings
        bool lockbool;

        //impl
        void process_dec(std::string text)
        {
            newasm::progwin::api::cout("Deco::"+text);
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
            }
            /*
            CONSTRUCTIVE DECORATORS
            */
            if(text == newasm::expcfg::decorators.at(newasm::expcfg::lock))
            {
                newasm::expcfg::lockbool = true;
                return;
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return;
        }
    }
}