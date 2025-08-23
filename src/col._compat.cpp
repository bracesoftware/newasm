// NewASM Virtual Machine

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