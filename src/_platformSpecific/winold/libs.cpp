// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#include <windows.h>
using LibHandle = HMODULE;
using FuncHandler = const char*(*)();
LibHandle loadLibrary(const char* libname)
{
    return LoadLibraryA(libname);
}

void* loadFunction(LibHandle lib, const char* funcname)
{
    return (void*)GetProcAddress(lib, funcname);
}

void closeLibrary(LibHandle lib)
{
    FreeLibrary(lib);
}

namespace newasm 
{
    LibHandle library = nullptr;
    FuncHandler funcHandler = nullptr;
}


namespace newasm
{
    namespace kernel
    {
        namespace dynamic
        {
            int CALL(std::string libname, std::string func)
            {
                std::filesystem::path base = std::filesystem::current_path();
                std::filesystem::path lib;

                lib = base / (newasm::header::functions::remq(libname) + ".dll");

                std::string libname_OS = lib.string();
                newasm::progwin::api::cout("Trying to load: " + libname_OS);

                if (!std::filesystem::exists(lib))
                {
                    newasm::progwin::api::cout("Library file does not exist at: " + libname_OS);
                    return 1;
                }

                newasm::library = loadLibrary(libname_OS.c_str());

                if(!newasm::library)
                {
                    newasm::progwin::api::cout("Cannot find the `" + libname_OS + "` library...");
                    return 1;
                }

                newasm::funcHandler = (FuncHandler)loadFunction(newasm::library, (std::string("__newasm_syscall_") + func).c_str());
                if(!newasm::funcHandler)
                {
                    newasm::progwin::api::cout("Cannot find the `" + func + "` syscall...");
                    closeLibrary(newasm::library);
                    return 1;
                }

                newasm::mem::regs::tlr.set_value(std::string(_NEWASM_CALL_FUNC()));

                closeLibrary(newasm::library);
                return 0;
            }

        }
    }
}