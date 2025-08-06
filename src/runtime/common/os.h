/*

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/


#define _NEWASM_OS_windows 0
#define _NEWASM_OS_linux 1
#define _NEWASM_OS_windows_old 2

#ifdef _WIN32_OLD
    #define _NEWASM_OS _NEWASM_OS_windows_old
#elif defined(_WIN32)
    #define _NEWASM_OS _NEWASM_OS_windows
#elif defined(__linux__)
    #define _NEWASM_OS _NEWASM_OS_linux
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
                #endif
            ;
        }
    }
}