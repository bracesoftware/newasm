// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

#if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
    #include <windows.h>
    using LibHandle = HMODULE;
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include <dlfcn.h>
    using LibHandle = void*;
#endif

module(dynlib, {
    //setup goes here
});

namespace newasm
{
    namespace dynlib
    {
        namespace err_codes
        {
            const int labels_unsupported = 1;
            const int unsupported_instruction = 2;

            const std::unordered_map<int, std::string> identifier = {
                {labels_unsupported, "LabelsUnsupported"},
                {unsupported_instruction, "UnsupportedInstruction"}
            };
        }
        namespace mem
        {
            std::vector<std::string> invalid_dynlibs;
        }
        namespace settings
        {
            std::string analyzed_dynlib;
            std::string lastline;
            int lastlinedx;
        }
        namespace functions
        {
            inline void abort(int error_code) noexcept
            {
                newasm::dynlib::mem::invalid_dynlibs.push_back(newasm::dynlib::settings::analyzed_dynlib);
                std::cout << newasm::header::col::reset
                << newasm::header::col::red << "\t  "
                << "Aborted dynamic library implementation.\n"<< "\t  "
                << "Runtime error " << error_code << " \"" << newasm::header::col::gray << newasm::header::style::underline
                << newasm::dynlib::err_codes::identifier.at(error_code) << newasm::header::col::reset
                << newasm::header::col::red << "\" in " << newasm::header::col::gray << newasm::header::style::underline
                << newasm::dynlib::settings::analyzed_dynlib << ":" << newasm::dynlib::settings::lastlinedx << newasm::header::col::reset
                << newasm::header::col::red << " >> " << newasm::header::col::gray << newasm::dynlib::settings::lastline << "\n";
                return;
            }
        }
    }
    class DynamicLibrary final
    {
        private LibHandle handle = nullptr;
        std::string libn;

        public explicit inline DynamicLibrary(const std::string& name) : libn(name)
        {
            this->load(name);
        }

        explicit inline DynamicLibrary() {}

        inline ~DynamicLibrary()
        {
            this->unload();
        }

        inline auto getName() const
        {
            return libn;
        }

        inline bool loaded() const
        {
            return handle != nullptr;
        }

        inline void load(const std::string& name)
        {
            this->libn = name;
            #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
            handle = LoadLibraryA((name + ".dll").c_str());
            #elif _NEWASM_OS == _NEWASM_OS_linux
            handle = dlopen(("./lib" + name + ".so").c_str(), RTLD_LAZY);
            if(!handle) handle = dlopen(("./" + name + ".so").c_str(), RTLD_LAZY);
            #endif
        }

        inline void unload()
        {
            if(handle)
            {
                ReturnUnion returnVal;
                this->call<void>(DL_EVENT_SIG + "onexit"_str, &returnVal);
                #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
                FreeLibrary(handle);
                #elif _NEWASM_OS == _NEWASM_OS_linux
                dlclose(handle);
                #endif
                handle = nullptr;
            }
        }

        template<typename Ret, typename... Args>
        inline bool call(const std::string& funcName, Ret* ret, Args... args)
        {
            if(!handle) return false;
            #if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old
            auto proc = GetProcAddress(handle, funcName.c_str());
            #elif _NEWASM_OS == _NEWASM_OS_linux
            auto proc = dlsym(handle, funcName.c_str());
            #endif

            if(!proc)
            {
                return false;
            }

            auto func = reinterpret_cast<Ret(*)(Args...)>(proc);
            if constexpr(std::is_same_v<Ret, void>)
            {
                func(args...);
            }
            else if constexpr(std::is_same_v<Ret, ReturnUnion>)
            {
                ReturnUnion r;
                r = func(args...);
                *ret = r;
            }
            return true;
        }

        template<typename Ret, typename... Args>
        inline bool syscall(int id, Ret* ret, Args... args)
        {
            return call<Ret, Args...>(FOREIGN_SYSCALL_SIG + newasm::_std::to_string(id), ret, args...);
        }
    };
}