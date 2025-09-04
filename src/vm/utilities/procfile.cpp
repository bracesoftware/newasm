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
    inline void procfile(std::string filename) // process simple scripts
    {
        std::ifstream file(filename);
        if(!file.is_open())
        {
            std::cout << newasm::header::col::light_red << filename << " not found." << std::endl; 
            return;
        }

        std::string line;
        int idx = 0;
        while(std::getline(file, line))
        {
            //std::cout << "Processing line " << idx << " -> " << line << std::endl;
            try
            {
                newasm::procline(line);
            }
            catch(std::exception& e)
            {
                std::cerr << "stdlib te zajebucnuo stdlib:" << idx << " -> " << e.what() << std::endl;
            }
            idx ++;
        }

        file.close();
        return;
    }
}