// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_platformSpecific/win32/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_platformSpecific/linux/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_platformSpecific/winold/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_platformSpecific/android/libs.cpp"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_platformSpecific/macos/libs.cpp"
#endif