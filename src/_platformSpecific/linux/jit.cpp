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

#include <cstdio>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

#if _NEWASM_ARCH == _NEWASM_ARCH_x86_32bit
static void jit_print_x86_32(const char* text)
{
    unsigned char code[] = {
        0x68, 0,0,0,0,       // push imm32 (adresa stringa)
        0xB8, 0,0,0,0,       // mov eax, imm32 (adresa printf)
        0xFF, 0xD0,          // call eax
        0x83, 0xC4, 0x04,    // add esp,4
        0xC3                 // ret
    };

    *(uint32_t*)(code+1) = (uint32_t)text;
    *(uint32_t*)(code+6) = (uint32_t)printf;

    void* mem = mmap(nullptr, sizeof(code), PROT_READ|PROT_WRITE|PROT_EXEC,
                     MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    memcpy(mem, code, sizeof(code));

    using fn_t = void(*)();
    fn_t fn = (fn_t)mem;
    fn();

    munmap(mem, sizeof(code));
}
#elif _NEWASM_ARCH == _NEWASM_ARCH_x86_64bit
static void jit_print_x86_64(const char* text)
{
    unsigned char code[] = {
        0x48, 0xBF, 0,0,0,0,0,0,0,0,   // mov rdi, imm64 (string)
        0x48, 0xB8, 0,0,0,0,0,0,0,0,   // mov rax, imm64 (printf)
        0xFF, 0xD0,                     // call rax
        0xC3                            // ret
    };

    *(uint64_t*)(code+2) = (uint64_t)text;
    *(uint64_t*)(code+12) = (uint64_t)printf;

    void* mem = mmap(nullptr, sizeof(code), PROT_READ|PROT_WRITE|PROT_EXEC,
                     MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    memcpy(mem, code, sizeof(code));

    using fn_t = void(*)();
    fn_t fn = (fn_t)mem;
    fn();

    munmap(mem, sizeof(code));
}
#elif _NEWASM_ARCH == _NEWASM_ARCH_arm_32bit
static void jit_print_arm32(const char* text)
{
    unsigned int code[] = {
        0xe3a00000,             // mov r0, #0 placeholder
        0xe59f1000,             // ldr r1, [pc, #0] placeholder
        0xe12fff11,             // blx r1
        0xe12fff1e,             // bx lr
        0,                      // adresa printf
        0                       // adresa text
    };
    code[0] = 0xe3a00000 | ((uintptr_t)text & 0xFF);
    code[4] = (unsigned int)printf;
    code[5] = (unsigned int)text;

    void* mem = mmap(nullptr, sizeof(code), PROT_READ|PROT_WRITE|PROT_EXEC,
                     MAP_PRIVATE|MAP_ANONYMOUS, -1,0);
    memcpy(mem, code, sizeof(code));

    using fn_t = void(*)();
    fn_t fn = (fn_t)mem;
    fn();

    munmap(mem, sizeof(code));
}
#elif _NEWASM_ARCH == _NEWASM_ARCH_arm_64bit
static void jit_print_arm64(const char* text)
{
    uint32_t code[] = {
        0x58000040, // LDR X0, #offset_text
        0x58000041, // LDR X1, #offset_printf (placeholder)
        0x94000000, // BL printf (placeholder)
        0xD65F03C0, // RET
        0,          // printf address
        0           // text address
    };

    code[4] = (uint64_t)printf;
    code[5] = (uint64_t)text;

    void* mem = mmap(nullptr, sizeof(code), PROT_READ|PROT_WRITE|PROT_EXEC,
                     MAP_PRIVATE|MAP_ANONYMOUS, -1,0);
    memcpy(mem, code, sizeof(code));

    using fn_t = void(*)();
    fn_t fn = (fn_t)mem;
    fn();

    munmap(mem, sizeof(code));
}
#endif

namespace newasm
{
    void jit_print(const char* text)
    {
        #if _NEWASM_ARCH == _NEWASM_ARCH_x86_32bit
            jit_print_x86_32(text);
        #elif _NEWASM_ARCH == _NEWASM_ARCH_x86_64bit
            jit_print_x86_64(text);
        #elif _NEWASM_ARCH == _NEWASM_ARCH_arm_32bit
            jit_print_arm32(text);
        #elif _NEWASM_ARCH == _NEWASM_ARCH_arm_64bit
            jit_print_arm64(text);
        #endif
    }
}