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


static void send_req(const std::string& msg)
{
    std::string path = newasm::core::constants::data_folder+
        newasm::core::constants::separator+
        newasm::core::constants::cache_folder+
        newasm::core::constants::separator+
        newasm::core::constants::ipc_file;
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    out.write(msg.c_str(), msg.size());
    out.close();

    while(std::ifstream(path)) //wait for response
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

//usable api
namespace newasm
{
    namespace progwin
    {
        namespace api
        {
            void cout(std::string text)
            {
                send_req("cout:"+text);
            }
            void exit()
            {
                send_req("exit:0");
            }
        }
    }
}