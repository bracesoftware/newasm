/*

The ORIGINAL CODE is the `NewASM Program Window` Source Code.
The INITIAL DEVELOPER is Brace Software Co., DEntisT.
The COPYRIGHT YEAR is 2024.

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

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <cstdio> // za remove()

namespace newasm
{
    namespace ipc
    {
        const std::string separator =
        #ifdef _WIN32
            "\\"
        #else
            "/"
        #endif
        ;
        const std::string fileloc = ".newasm" + separator + "_cache" + separator;
        const std::string ipc_file = "ipc._sys";

        namespace impl
        {
            std::vector<std::string> split_fixed(const std::string &str, char delimiter)
            {
                size_t pos = str.find(delimiter);
                std::vector<std::string> tokens;

                if(pos != std::string::npos)
                {
                    tokens.push_back(str.substr(0, pos));
                    tokens.push_back(str.substr(pos + 1));
                }
                else
                {
                    tokens.push_back(str);
                }
                
                return tokens;
            }

            void pause() {
            #ifdef _WIN32
                system("pause");
            #else
                std::cout << "Press enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            #endif
            }

        }

        namespace cmd
        {
            const std::string cout = "cout";
            const std::string exit = "exit";
        }
    }
}

int main()
{
    std::string path_ = newasm::ipc::fileloc + newasm::ipc::ipc_file;
    while (true)
    {
        std::ifstream in(
            path_.c_str(),
            std::ios::binary
        );
        if(in)
        {
            std::string request((std::istreambuf_iterator<char>(in)), {});
            in.close();

            if(request.find(':') == std::string::npos)
            {
                return 1;
            }

            std::vector<std::string> cmd = newasm::ipc::impl::split_fixed(request, ':');
            if(cmd[0] == newasm::ipc::cmd::cout)
            {
                std::cout << cmd[1];
            }
            if(cmd[0] == newasm::ipc::cmd::exit)
            {
                if(cmd[1] == "0")
                {
                    newasm::ipc::impl::pause();
                    return 1;
                }
            }


            std::remove(path_.c_str());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // spriječi 100% CPU
    }
    return 1;
}
