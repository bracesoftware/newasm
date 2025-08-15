// NewASM Virtual Machine

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_compat/win32/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_compat/linux/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_compat/winold/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_compat/android/libs.cpp"
#endif