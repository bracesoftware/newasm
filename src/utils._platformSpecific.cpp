// NewASM Virtual Machine

#if _NEWASM_OS == _NEWASM_OS_windows
    #include "_platformSpecific/win32/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_linux
    #include "_platformSpecific/linux/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_windows_old
    #include "_platformSpecific/winold/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_android
    #include "_platformSpecific/android/utils.h"
#elif _NEWASM_OS == _NEWASM_OS_macos
    #include "_platformSpecific/macos/utils.h"
#endif