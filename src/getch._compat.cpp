// NewASM Virtual Machine

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_compat/win32/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_compat/linux/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_compat/winold/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_compat/android/getch.cpp"
#endif