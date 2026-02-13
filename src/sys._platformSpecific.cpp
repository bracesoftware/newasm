// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

// Colors
#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_platformSpecific/win32/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_platformSpecific/linux/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_platformSpecific/winold/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_platformSpecific/android/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_platformSpecific/macos/col.cpp"
#endif

// Save folder
#if _NEWASM_OS == _NEWASM_OS_windows
    #include <shlobj.h>
    #include "_platformSpecific/win32/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_platformSpecific/linux/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include <shlobj.h>
    #include "_platformSpecific/winold/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_platformSpecific/android/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_platformSpecific/macos/data.cpp"
#endif

// JIT COMPILER
#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_platformSpecific/win32/jit.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_platformSpecific/linux/jit.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_platformSpecific/winold/jit.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_platformSpecific/android/jit.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_platformSpecific/macos/jit.cpp"
#endif

__newasm_LOAD_PACKAGE_MODULE(hostos_specific__SYS, {
    //setup goes here
});

#if _NEWASM_OS == _NEWASM_OS_windows

    #define __NEWASM_inlineMenu___1(__1) CONSOLE_SCREEN_BUFFER_INFO csbi;\
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);\
        int __1 = csbi.dwCursorPosition.Y;

    #define __NEWASM_inlineMenu___2(__1, __2) for(int i = 0; i < __2.size(); ++i){newasm::_compat::clear_line(__1 + i);\
        newasm::_compat::set_cursor(0, __1 + i);if(i == selected)std::cout << "> " << __2[i]; \
        else std::cout << "  " << __2[i];}
    
    #define __NEWASM_inlineMenu___3(__1, __2) for(int i = 0; i < __2.size(); ++i)newasm::_compat::clear_line(__1 + i);

#elif _NEWAS_OS == _NEWASM_OS_linux

    #define __NEWASM_inlineMenu___1(__1) std::cout << "\033[s";

    #define __NEWASM_inlineMenu___2(__1, __2) std::cout << "\033[u";for(int i = 0; i < __2.size(); ++i)\
        {if(i == selected) std::cout << "> " << __2[i] << "  \033[K\n";\
        else std::cout << "  " << __2[i] << "  \033[K\n";}

    #define __NEWASM_inlineMenu___3(__1, __2) std::cout << "\033[u"; \
            for(size_t i = 0; i < __2.size(); ++i)std::cout << "\033[K\n";\
            std::cout << "\033[u";

#endif