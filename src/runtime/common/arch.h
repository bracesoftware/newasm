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

#define _NEWASM_ARCH_x86_32bit 0
#define _NEWASM_ARCH_x86_64bit 1
#define _NEWASM_ARCH_arm_32bit 2
#define _NEWASM_ARCH_arm_64bit 3
#define _NEWASM_ARCH_wtf 4

#if defined(__x86_64__) || defined(__amd64__)
    #define _NEWASM_ARCH _NEWASM_ARCH_x86_64bit
#elif defined(__i386__)
    #define _NEWASM_ARCH _NEWASM_ARCH_x86_32bit
#elif defined(__aarch64__)
    #define _NEWASM_ARCH _NEWASM_ARCH_arm_64bit
#elif defined(__arm__)
    #define _NEWASM_ARCH _NEWASM_ARCH_arm_32bit
#else
    #define _NEWASM_ARCH _NEWASM_ARCH_wtf
#endif