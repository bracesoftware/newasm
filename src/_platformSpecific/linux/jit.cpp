// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#include <cstdio>
#include <cstring>
#include <sys/mman.h>
#include <dlfcn.h>
#include <unistd.h>

#if _NEWASM_ARCH == _NEWASM_ARCH_x86_32bit
static void jit_print_x86_32(const char* text)
{
    void* real_printf = dlsym(RTLD_DEFAULT, "printf");

    unsigned char code[] = {
        0x68, 0,0,0,0,        // push text
        0x31, 0xC0,           // xor eax, eax  (variadic ABI)
        0xB8, 0,0,0,0,        // mov eax, printf
        0xFF, 0xD0,           // call eax
        0x83, 0xC4, 0x04,     // add esp, 4
        0xC3                  // ret
    };

    *(uint32_t*)(code + 1) = (uint32_t)text;
    *(uint32_t*)(code + 7) = (uint32_t)real_printf;

    void* mem = mmap(nullptr, sizeof(code),
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    memcpy(mem, code, sizeof(code));
    ((void(*)())mem)();
    munmap(mem, sizeof(code));
}
#elif _NEWASM_ARCH == _NEWASM_ARCH_x86_64bit
static void jit_print_x86_64(const char* text)
{
    void* real_printf = dlsym(RTLD_DEFAULT, "printf");

    unsigned char code[] = {
        0x48, 0x83, 0xEC, 0x18,        // sub rsp, 24
        0x48, 0xBF, 0,0,0,0,0,0,0,0,   // mov rdi, text
        0x31, 0xC0,                   // xor eax, eax
        0x48, 0xB8, 0,0,0,0,0,0,0,0,   // mov rax, real_printf
        0xFF, 0xD0,                   // call rax
        0x48, 0x83, 0xC4, 0x18,        // add rsp, 24
        0xC3                          // ret
    };

    *(uint64_t*)(code + 6)  = (uint64_t)text;
    *(uint64_t*)(code + 18) = (uint64_t)real_printf;

    void* mem = mmap(nullptr, sizeof(code),
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    memcpy(mem, code, sizeof(code));

    ((void(*)())mem)();
    munmap(mem, sizeof(code));
}

#elif _NEWASM_ARCH == _NEWASM_ARCH_arm_32bit
static void jit_print_arm32(const char* text)
{
    uint32_t code[] = {
        0xE59F0008,   // ldr r0, [pc, #8]   ; text
        0xE59F1008,   // ldr r1, [pc, #8]   ; printf
        0xE12FFF11,   // blx r1
        0xE12FFF1E,   // bx lr
        (uint32_t)text,
        (uint32_t)printf
    };

    void* mem = mmap(nullptr, sizeof(code),
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    memcpy(mem, code, sizeof(code));

    // ARM OBAVEZNO:
    __builtin___clear_cache(
        (char*)mem,
        (char*)mem + sizeof(code)
    );

    ((void(*)())mem)();
    munmap(mem, sizeof(code));
}
#elif _NEWASM_ARCH == _NEWASM_ARCH_arm_64bit
static void jit_print_arm64(const char* text)
{
    uint32_t code[] = {
        0x58000040,   // ldr x0, literal (text)
        0x58000061,   // ldr x1, literal (printf)
        0xD63F0020,   // blr x1
        0xD65F03C0,   // ret
        (uint32_t)((uint64_t)text),
        (uint32_t)((uint64_t)printf)
    };

    void* mem = mmap(nullptr, sizeof(code),
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    memcpy(mem, code, sizeof(code));

    __builtin___clear_cache(
        (char*)mem,
        (char*)mem + sizeof(code)
    );

    ((void(*)())mem)();
    munmap(mem, sizeof(code));
}
#endif

namespace newasm
{
    void jit_print(const char* text)
    {
        std::cout << text;
        return;
    }
    void jit_print__(const char* text)
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