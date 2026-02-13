// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

// Colors
#if _NEWASM_OS == _NEWASM_OS_windows
    link "_platformSpecific/win32/col";
#elif _NEWASM_OS == _NEWASM_OS_linux
    link "_platformSpecific/linux/col";
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    link "_platformSpecific/winold/col";
#elif _NEWASM_OS == _NEWASM_OS_android
    link "_platformSpecific/android/col";
#elif _NEWASM_OS == _NEWASM_OS_macos
    link "_platformSpecific/macos/col";
#endif

// Save folder
#if _NEWASM_OS == _NEWASM_OS_windows
    #include <shlobj.h>
    link "_platformSpecific/win32/data";
#elif _NEWASM_OS == _NEWASM_OS_linux
    link "_platformSpecific/linux/data";
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include <shlobj.h>
    link "_platformSpecific/winold/data";
#elif _NEWASM_OS == _NEWASM_OS_android
    link "_platformSpecific/android/data";
#elif _NEWASM_OS == _NEWASM_OS_macos
    link "_platformSpecific/macos/data";
#endif

// JIT COMPILER
#if _NEWASM_OS == _NEWASM_OS_windows
    link "_platformSpecific/win32/jit";
#elif _NEWASM_OS == _NEWASM_OS_linux
    link "_platformSpecific/linux/jit";
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    link "_platformSpecific/winold/jit";
#elif _NEWASM_OS == _NEWASM_OS_android
    link "_platformSpecific/android/jit";
#elif _NEWASM_OS == _NEWASM_OS_macos
    link "_platformSpecific/macos/jit";
#endif

__newasm_LOAD_PACKAGE_MODULE(hostos_specific__SYS, {
    //setup goes here
});

//SOTS=selected option text style,btw
#define __NEWASM_inlineMenu___SOTS (newasm::header::col::reset + newasm::header::style::bold + newasm::header::col::green)
//NSOTS=non selected option text style btw
#define __NEWASM_inlineMenu___NSOTS (newasm::header::col::reset + newasm::header::col::gray)
//CS!=counter strike, CS=cursor style btw
#define __NEWASM_inlineMenu___CS (newasm::header::col::reset)

#if _NEWASM_OS == _NEWASM_OS_windows || _NEWASM_OS == _NEWASM_OS_windows_old

    #define __NEWASM_inlineMenu___1(__1) CONSOLE_SCREEN_BUFFER_INFO csbi;\
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);\
        int __1 = csbi.dwCursorPosition.Y;

    #define __NEWASM_inlineMenu___2(__1, __2) for(int i = 0; i < __2.size(); ++i){newasm::_compat::clear_line(__1 + i);\
        newasm::_compat::set_cursor(0, __1 + i);if(i == selected)std::cout << __NEWASM_inlineMenu___CS<<"> " << __NEWASM_inlineMenu___SOTS << __2[i] << '\n'; \
        else std::cout << "  " << __NEWASM_inlineMenu___NSOTS << __2[i] << '\n';}
    
    #define __NEWASM_inlineMenu___3(__1, __2) for(int i = 0; i < __2.size(); ++i)newasm::_compat::clear_line(__1 + i);

    #define __NEWASM_inlineMenu___4

#elif _NEWASM_OS == _NEWASM_OS_linux

    #define __NEWASM_inlineMenu___1(__1) std::cout << "\033[s";int __1##__LINE__##__COUNTER__=0;

    #define __NEWASM_inlineMenu___2(__1, __2) int __1##__LINE__##__COUNTER__=0;std::cout << "\033[u";for(int i = 0; i < __2.size(); ++i)\
        {if(i == selected) std::cout << __NEWASM_inlineMenu___CS<<"> " <<__NEWASM_inlineMenu___SOTS<< __2[i] << "  \033[K\n";\
        else std::cout << "  " << __NEWASM_inlineMenu___NSOTS<<__2[i] << "  \033[K\n";}

    #define __NEWASM_inlineMenu___3(__1, __2) std::cout << "\033[u"; \
        for(size_t i = 0; i < __2.size(); ++i)std::cout << "\033[K\n";\
        std::cout << "\033[u";int __1##__LINE__##__COUNTER__=0;

    #define __NEWASM_inlineMenu___4

#endif