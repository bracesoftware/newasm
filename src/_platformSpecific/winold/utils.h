// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

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
