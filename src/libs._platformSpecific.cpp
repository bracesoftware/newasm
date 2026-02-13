// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#if _NEWASM_OS == _NEWASM_OS_windows
    link "_platformSpecific/win32/libs";
#elif _NEWASM_OS == _NEWASM_OS_linux
    link "_platformSpecific/linux/libs";
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    link "_platformSpecific/winold/libs";
#elif _NEWASM_OS == _NEWASM_OS_android
    link "_platformSpecific/android/libs";
#elif _NEWASM_OS == _NEWASM_OS_macos
    link "_platformSpecific/macos/libs";
#endif