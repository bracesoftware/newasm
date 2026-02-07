// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#if _NEWASM_ARCH == _NEWASM_ARCH_x86_32bit

static void jit_print_win_x86_32(const char* text)
{
    unsigned char code[] = {
        0x68, 0,0,0,0,       // push imm32 (address of string)
        0xB8, 0,0,0,0,       // mov eax, imm32 (address of printf)
        0xFF, 0xD0,          // call eax
        0x83, 0xC4, 0x04,    // add esp,4
        0xC3                 // ret
    };

    *(DWORD*)(code + 1) = (DWORD)text;
    *(DWORD*)(code + 6) = (DWORD)printf;

    void* mem = VirtualAlloc(nullptr, sizeof(code), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(mem, code, sizeof(code));

    using fn_t = void(*)();
    fn_t fn = (fn_t)mem;
    fn();

    VirtualFree(mem, 0, MEM_RELEASE);
}

#elif _NEWASM_ARCH == _NEWASM_ARCH_x86_64bit

static void jit_print_win_x86_64(const char* text)
{
    unsigned char code[] = {
        0x48, 0xB9, 0,0,0,0,0,0,0,0, // mov rcx, imm64 (ptr string)
        0x48, 0xB8, 0,0,0,0,0,0,0,0, // mov rax, imm64 (ptr printf)
        0xFF, 0xD0,                   // call rax
        0xC3                          // ret
    };

    *(uint64_t*)(code + 2) = (uint64_t)text;
    *(uint64_t*)(code + 12) = (uint64_t)printf;

    void* mem = VirtualAlloc(nullptr, sizeof(code), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(mem, code, sizeof(code));

    using fn_t = void(*)();
    fn_t fn = (fn_t)mem;
    fn();

    VirtualFree(mem, 0, MEM_RELEASE);
}

#endif

namespace newasm
{
    void jit_print(const char* text)
    {
        #if _NEWASM_ARCH == _NEWASM_ARCH_x86_32bit
            jit_print_win_x86_32(text);
        #elif _NEWASM_ARCH == _NEWASM_ARCH_x86_64bit
            jit_print_win_x86_64(text);
        #endif
    }
}