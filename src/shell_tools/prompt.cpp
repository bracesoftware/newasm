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
    namespace shell_tools
    {
        inline bool prompt(std::string text)
        {
            std::string input;
            std::cout << newasm::header::col::light_blue << ":: " << newasm::header::col::gray << text << newasm::header::col::light_blue << " [y/n] ";
            std::cout << newasm::header::col::reset;
            std::getline(std::cin, input);
            //std::cout << std::endl;

            if((input == "Y") or (input == "y")) return true;
            if((input == "N") or (input == "n"))
            {
                newasm::header::functions::err("Operation cancelled successfully.");
                return false;
            }
            newasm::header::functions::err("Unknown option. Operation cancelled.");
            return false;
        }
    }
}