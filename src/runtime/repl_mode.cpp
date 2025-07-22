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
    int repl()
    {
        std::string line;
        std::cout << "\n";
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while(true)
        {
            line.clear();
            std::cout << newasm::header::style::underline + newasm::header::col::gray;
            std::getline(std::cin,line);
            
            newasm::header::data::lastln = line;
            std::cout << newasm::header::col::reset;
            
            newasm::header::data::lastlndx++;
            newasm::process << line;

            if(newasm::header::data::repl_end)
            {
                break;
            }
        }
        return 0;
    }
}