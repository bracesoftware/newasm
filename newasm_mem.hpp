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
    namespace exit_codes
    {
        const int invalid_label = 1;
        const int invaid_proc = 2;
        const int invalid_retn = 3;
        const int sysreq_fail = 4;
        const int stk_underflow = 5;
        const int data_overflow = 6;
        const int dtyp_mismatch = 7;
    }
    namespace datatypes
    {
        const int number = 1;
        const int decimal = 2;
        const int text = 3;
    }
    namespace constv
    {
        const std::string pxstr = "Program finished with exit code : ";

        const int __data = 0b1;
        const int __start = 0b10;
    }
    namespace system
    {
        bool terminated = false;
        int label = 0;
        int stop = 0;
        int stoproc = 0;
        std::string cproc;
    }
    namespace mem
    {
        //fake registers lol
        namespace regs
        {
            //NORMAL REGISTERS
            int exc = 0;
            int fdx = 0;
            std::string tlr = "null";
            std::string psx = "null";
        }
        std::unordered_map<std::string, std::string> data;
        std::unordered_map<std::string, int> datatypes;
        std::unordered_map<std::string, std::vector<std::string>> funcs;
        std::vector<std::string> stack;
        namespace functions
        {
            template<typename T> bool datavalid(std::string dataname, T &dat)
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