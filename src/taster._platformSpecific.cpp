// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#if _NEWASM_OS == _NEWASM_OS_windows
    link "_platformSpecific/win32/getch";
#elif _NEWASM_OS == _NEWASM_OS_linux
    link "_platformSpecific/linux/getch";
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    link "_platformSpecific/winold/getch";
#elif _NEWASM_OS == _NEWASM_OS_android
    link "_platformSpecific/android/getch";
#elif _NEWASM_OS == _NEWASM_OS_macos
    link "_platformSpecific/macos/getch";
#endif

#if _NEWASM_OS == _NEWASM_OS_windows
    link "_platformSpecific/win32/screen";
#elif _NEWASM_OS == _NEWASM_OS_linux
    link "_platformSpecific/linux/screen";
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    link "_platformSpecific/winold/screen";
#elif _NEWASM_OS == _NEWASM_OS_android
    link "_platformSpecific/android/screen";
#elif _NEWASM_OS == _NEWASM_OS_macos
    link "_platformSpecific/macos/screen";
#endif