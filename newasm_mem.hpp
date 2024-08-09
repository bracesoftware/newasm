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

Portions created by the Initial Developer are Copyright (c) 2022
the Initial Developer. All Rights Reserved.

*/

namespace newasm
{
    namespace constv
    {
        const int __data = 0b1;
        const int __start = 0b10;
    }
    namespace system
    {
        bool terminated = false;
        int label = 0;
        int stop = 0;
        std::string cproc;
    }
    namespace mem
    {
        //fake registers lol
        namespace regs
        {
            //Unused registers
            int pri = 0;
            int alt = 0;
            //Used registers
            int fdx = 0;

            //Output registers
            int onm = 0;
            std::string otx = "";
        }
        std::unordered_map<std::string, std::string> data;
        std::unordered_map<std::string, std::vector<std::string>> funcs;
        namespace functions
        {
            bool datavalid(std::string dataname,std::unordered_map<std::string, std::string> &dat)
            {
                for(auto it = dat.begin(); it != dat.end(); it++)
                {
                    if(it->first == dataname)
                    {
                        return true;
                    }
                }
                return false;
            }
        }
    }
}