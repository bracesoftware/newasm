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

#include <dlfcn.h>
using LibHandle = void*;
using FuncHandler = const char*(*)();
LibHandle loadLibrary(const char* libname)
{
    return dlopen(libname, RTLD_LAZY);
}

void* loadFunction(LibHandle lib, const char* funcname)
{
    return dlsym(lib, funcname);
}

void closeLibrary(LibHandle lib)
{
    dlclose(lib);
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

                lib = base / (newasm::header::functions::remq(libname) + ".so");

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