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


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

// get shi from go
extern "C"
{
    int send_tcp(const char* addr, const char* msg);
    char* recv_tcp(const char* addr);
    void tcp_init();
    void free_string(char* str);
}


namespace newasm
{
    namespace syscalls
    {
        namespace tcp
        {
            int send(std::string addr, std::string content)
            {
                auto res = send_tcp(addr.c_str(), content.c_str());
                return res;
            }
            std::string recv(std::string addr)
            {
                char* res = recv_tcp(addr.c_str());
                std::string result(res);
                free_string(res);
                return result;
            }
            void init()
            {
                tcp_init();
                return;
            }
        }
    }
}