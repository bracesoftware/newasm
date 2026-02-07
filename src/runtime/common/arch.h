// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

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