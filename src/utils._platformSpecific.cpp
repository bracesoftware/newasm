// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(hostos_specific__UTILS, {
    //setup goes here
});


#if _NEWASM_OS == _NEWASM_OS_windows
    link "_platformSpecific/win32/utils";
#elif _NEWASM_OS == _NEWASM_OS_linux
    link "_platformSpecific/linux/utils";
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    link "_platformSpecific/winold/utils";
#elif _NEWASM_OS == _NEWASM_OS_android
    link "_platformSpecific/android/utils";
#elif _NEWASM_OS == _NEWASM_OS_macos
    link "_platformSpecific/macos/utils";
#endif

link "vm/_experimental";