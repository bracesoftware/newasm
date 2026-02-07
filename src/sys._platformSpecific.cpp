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