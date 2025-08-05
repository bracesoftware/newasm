#include <iostream>

#ifdef _WIN32
  #include <windows.h>
  using LibHandle = HMODULE;

  LibHandle loadLibrary(const char* libname) {
      return LoadLibraryA(libname);
  }

  void* loadFunction(LibHandle lib, const char* funcname) {
      return (void*)GetProcAddress(lib, funcname);
  }

  void closeLibrary(LibHandle lib) {
      FreeLibrary(lib);
  }

#else // Linux/macOS
  #include <dlfcn.h>
  using LibHandle = void*;

  LibHandle loadLibrary(const char* libname) {
      return dlopen(libname, RTLD_LAZY);
  }

  void* loadFunction(LibHandle lib, const char* funcname) {
      return dlsym(lib, funcname);
  }

  void closeLibrary(LibHandle lib) {
      dlclose(lib);
  }

#endif

using FuncHandler = std::string(*)();
namespace newasm 
{
    LibHandle library = nullptr;
    FuncHandler funcHandler = nullptr;
}

#define _NEWASM_CALL_FUNC() (newasm::funcHandler ? newasm::funcHandler() : "err")


namespace newasm
{
    namespace kernel
    {
        namespace dynamic
        {
            int CALL(std::string libname, std::string func)
            {
                std::string libname_OS = (
                #ifdef _WIN32
                    newasm::header::functions::remq(libname) + ".dll"
                #else
                    "./" + newasm::header::functions::remq(libname) + ".so"
                #endif
                );

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

                newasm::mem::regs::tlr.set_value(_NEWASM_CALL_FUNC());

                closeLibrary(newasm::library);
                return 0;
            }

        }
    }
}