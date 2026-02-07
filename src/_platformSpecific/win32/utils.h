// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

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
