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


#include <windows.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

//

namespace newasm
{
    namespace net
    {
        bool download(const std::string& url, const std::string& output_path)
        {
            std::wstring wurl(url.begin(), url.end());
            std::wstring woutput(output_path.begin(), output_path.end());
            HRESULT res = URLDownloadToFileW(NULL, wurl.c_str(), woutput.c_str(), 0, NULL);
            return res == S_OK;
        }
    }
    namespace runtime
    {
        bool start_program(const std::string& path)
        {
            std::string full_path = std::filesystem::current_path().string() + newasm::core::constants::separator + path;
            if(!std::filesystem::exists(full_path))
            {
                std::cerr << "Cannot create a process -> `"<< full_path << "` not found.\n";
                return false;
            }
            ShellExecute(NULL, "open", full_path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
            return true;
            /*STARTUPINFO si = { sizeof(si) };
            PROCESS_INFORMATION pi;

            std::string cmd = "\"" + path + "\"";
            BOOL success = CreateProcessA(
                NULL,               // application name
                cmd.data(),         // command line
                NULL, NULL,         // process/thread security
                FALSE,              // handle inheritance
                CREATE_NEW_CONSOLE, // creation flags
                NULL,               // environment
                NULL,               // current directory
                &si, &pi            // startup and process info
            );

            if (success) {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }

            return success;*/
        }
    }
}
