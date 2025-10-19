// NewASM Virtual Machine

// Colors
#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_compat/win32/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_compat/linux/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_compat/winold/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_compat/android/col.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_compat/macos/col.cpp"
#endif

// Save folder
#if _NEWASM_OS == _NEWASM_OS_windows
    #include <shlobj.h>
    #include "_compat/win32/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_compat/linux/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include <shlobj.h>
    #include "_compat/winold/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_compat/android/data.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_compat/macos/data.cpp"
#endif