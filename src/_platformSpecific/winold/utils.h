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
#include <wininet.h>
#include <string>
#include <iostream>

#pragma comment(lib, "wininet.lib")

namespace newasm
{
    namespace net
    {
        bool download(const std::string& url, const std::string& output_path)
        {
            HINTERNET hInternet = InternetOpenA("NewASM Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (!hInternet)
                return false;

            HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
            if (!hFile)
            {
                InternetCloseHandle(hInternet);
                return false;
            }

            HANDLE hOutput = CreateFileA(output_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hOutput == INVALID_HANDLE_VALUE)
            {
                InternetCloseHandle(hFile);
                InternetCloseHandle(hInternet);
                return false;
            }

            char buffer[4096];
            DWORD bytesRead, bytesWritten;
            bool success = true;

            while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
            {
                if (!WriteFile(hOutput, buffer, bytesRead, &bytesWritten, NULL))
                {
                    success = false;
                    break;
                }
            }

            CloseHandle(hOutput);
            InternetCloseHandle(hFile);
            InternetCloseHandle(hInternet);

            return success;
        }
    }

    namespace runtime
    {
        bool start_program(const std::string& path)
        {
            SHELLEXECUTEINFOA execInfo;
            memset(&execInfo, 0, sizeof(execInfo));
            execInfo.cbSize = sizeof(execInfo);
            execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            execInfo.lpVerb = "open";
            execInfo.lpFile = path.c_str();
            execInfo.nShow = SW_SHOWNORMAL;

            HINSTANCE result = ShellExecuteA(NULL, execInfo.lpVerb, execInfo.lpFile, NULL, NULL, execInfo.nShow);
            return reinterpret_cast<intptr_t>(result) > 32;
        }
    }
}
