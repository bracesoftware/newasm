// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#include "runtime/common/os.h"

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <cstdio> // FOR remove()

#if _NEWASM_OS == _NEWASM_OS_windows
#include <windows.h>
namespace newasm
{
    inline void enable_ansi() noexcept
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if(hOut == INVALID_HANDLE_VALUE)
        {
            return;
        }

        DWORD mode = 0;
        if(!GetConsoleMode(hOut, &mode))
        {
            return;
        }

        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        return;
    }
}
#endif

namespace newasm
{
    namespace header
    {
        namespace col
        {
            #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_linux
            const std::string red = "\033[31m";
            const std::string green = "\033[32m";
            const std::string yellow = "\033[33m";
            const std::string blue = "\033[34m";
            const std::string magenta = "\033[35m";
            const std::string cyan = "\033[36m";
            const std::string gray = "\033[90m";

            const std::string reset = "\033[0m";
            #elif _NEWASM_OS == _NEWASM_OS_windows_old
            const std::string red = "";
            const std::string green = "";
            const std::string yellow = "";
            const std::string blue = "";
            const std::string magenta = "";
            const std::string cyan = "";
            const std::string gray = "";

            const std::string reset = "";
            #endif
        }
        namespace style
        {
            #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_linux
            const std::string underline = "\033[4m";
            const std::string bold = "\033[1m";
            #elif _NEWASM_OS == _NEWASM_OS_windows_old
            const std::string underline = "";
            const std::string bold = "";
            #endif
        }

        namespace system_info
        {
            const std::string name = "[NewASM]";
            const std::string copyright_claim = "(c) 2025 Brace Software Co.";
            const std::string fullname = "NewASM Virtual Machine";
        }
    }
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

            void print(std::string text)
            {
                std::cout << newasm::header::col::magenta << "debug: sys -> " << newasm::header::col::gray << text << newasm::header::col::reset << std::endl;
            }

            void pause()
            {
                newasm::ipc::impl::print("Press enter to exit the system...");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    #if _NEWASM_OS == _NEWASM_OS_windows
    newasm::enable_ansi();
    #endif
    newasm::ipc::impl::print("Booting...");
    std::string path_ = newasm::ipc::fileloc + newasm::ipc::ipc_file;
    while(true)
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
                std::cout << newasm::header::col::magenta << "debug: vm -> " << newasm::header::col::gray << cmd[1];
                std::cout << newasm::header::col::reset;
            }
            if(cmd[0] == newasm::ipc::cmd::exit)
            {
                if(cmd[1] == "0")
                {
                    std::remove(path_.c_str());
                    return 1;
                }
            }


            std::remove(path_.c_str());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // prevent 100% CPU
    }
    return 1;
}