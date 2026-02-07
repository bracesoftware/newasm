// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


#define _NEWASM_OS_windows 0
#define _NEWASM_OS_linux 1
#define _NEWASM_OS_windows_old 2
#define _NEWASM_OS_android 3
#define _NEWASM_OS_macos 4

#ifdef _WIN32_OLD
    #define _NEWASM_OS _NEWASM_OS_windows_old
#elif defined(__android__)
    #define _NEWASM_OS _NEWASM_OS_android
#elif defined(_WIN32)
    #define _NEWASM_OS _NEWASM_OS_windows
#elif defined(__linux__)
    #define _NEWASM_OS _NEWASM_OS_linux
#elif defined(__APPLE__)
    #define _NEWASM_OS _NEWASM_OS_macos
#endif


namespace newasm
{
    namespace common
    {
        namespace os
        {
            const int windows = 0;
            const int linux = 1;
            const int windows_old = 2;
            const int android = 3;
            const int macos = 4;
        }

        int getos()
        {
            return
                #if _NEWASM_OS == _NEWASM_OS_windows
                    newasm::common::os::windows
                #elif _NEWASM_OS == _NEWASM_OS_windows_old
                    newasm::common::os::windows_old
                #elif _NEWASM_OS == _NEWASM_OS_linux
                    newasm::common::os::linux
                #elif _NEWASM_OS == _NEWASM_OS_android
                    newasm::common::os::android
                #elif _NEWASM_OS == _NEWASM_OS_macos
                    newasm::common::os::android
                #endif
            ;
        }
    }
}