// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

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
        inline bool startProgramOld(const std::string& path)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                execl(path.c_str(), path.c_str(), NULL);
                _exit(1);
            }
            else if (pid > 0)
            {
                return true; // roditelj
            }
            else
            {
                return false; // greška
            }
            return false;
        }
        inline bool startProgramNew(const std::string& path)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                execlp(
                    "x-terminal-emulator",
                    "x-terminal-emulator",
                    "-e",
                    path.c_str(),
                    (char*)nullptr
                );
                _exit(1);
            }
            return pid > 0;
        }
        inline bool start_program(const std::string& path)
        {
            return startProgramNew(path);
        }
    }
}