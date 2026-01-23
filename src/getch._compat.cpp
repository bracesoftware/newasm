// NewASM Virtual Machine

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_Compat/win32/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_Compat/linux/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_Compat/winold/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_Compat/android/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_Compat/macos/getch.cpp"
#endif