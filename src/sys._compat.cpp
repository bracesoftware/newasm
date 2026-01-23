// NewASM Virtual Machine

// Colors
#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_Compat/win32/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_Compat/linux/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_Compat/winold/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_Compat/android/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_Compat/macos/col.cpp"
#endif

// Save folder
#if _NEWASM_OS == _NEWASM_OS_windows
    #include <shlobj.h>
    #include "_Compat/win32/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_Compat/linux/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include <shlobj.h>
    #include "_Compat/winold/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_Compat/android/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_Compat/macos/data.cpp"
#endif