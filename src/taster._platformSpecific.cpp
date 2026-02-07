// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_platformSpecific/win32/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_platformSpecific/linux/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_platformSpecific/winold/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_platformSpecific/android/getch.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_platformSpecific/macos/getch.cpp"
#endif