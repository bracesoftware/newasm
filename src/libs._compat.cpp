// NewASM Virtual Machine

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_Compat/win32/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_Compat/linux/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_Compat/winold/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_Compat/android/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_Compat/macos/libs.cpp"
#endif