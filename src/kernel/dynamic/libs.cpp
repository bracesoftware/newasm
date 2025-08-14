

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

#if _NEWASM_OS != _NEWASM_OS_android
using FuncHandler = std::string(*)();
namespace newasm 
{
    LibHandle library = nullptr;
    FuncHandler funcHandler = nullptr;
}

#define _NEWASM_CALL_FUNC() (newasm::funcHandler ? newasm::funcHandler() : "err")
#endif

namespace newasm
{
    namespace kernel
    {
        namespace dynamic
        {
            int CALL(std::string libname, std::string func)
            #if _NEWASM_OS == _NEWASM_OS_android
            {
                std::cout << "Failed to call -> " << libname << "::" << func << std::endl;
                return 1;
            }
            #else
            {
                std::filesystem::path base = std::filesystem::current_path();
                std::filesystem::path lib;

                #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
                    lib = base / (newasm::header::functions::remq(libname) + ".dll");
                #elif _NEWASM_OS == _NEWASM_OS_linux
                    lib = base / (newasm::header::functions::remq(libname) + ".so");
                #endif

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

                newasm::mem::regs::tlr.set_value(_NEWASM_CALL_FUNC());

                closeLibrary(newasm::library);
                return 0;
            }

        }
    }
}