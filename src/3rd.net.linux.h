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

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace newasm
{
    namespace net
    {
        bool download(const std::string& url, const std::string& output_path)
        {
            std::ostringstream command;
            command << "wget -q -O " << output_path << " " << url;
            int res = std::system(command.str().c_str());
            bool ret = res == 0;
            if(!ret)
            {
                command.str("");
                command.clear();
                command << "rm " << output_path;
                if(std::filesystem::exists(output_path))
                {
                    std::system(command.str().c_str());
                }
            }
            return ret;
        }
    }
    namespace runtime
    {
        bool start_program(const std::string& path) {
            pid_t pid = fork();
            if (pid == 0) {
                // dijete
                execl(path.c_str(), path.c_str(), NULL);
                _exit(1); // ako execl ne uspije
            } else if (pid > 0) {
                return true; // roditelj
            } else {
                return false; // greška
            }
        }
    }
}