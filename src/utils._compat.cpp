// NewASM Virtual Machine

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_Compat/win32/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_Compat/linux/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_Compat/winold/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_Compat/android/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_Compat/macos/utils.h"
#endif