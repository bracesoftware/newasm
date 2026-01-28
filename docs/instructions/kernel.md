# Kernel instructions
These instructions are used to manipulate with the kernel and access its functions.
Following instructions will be covered in the article:
1. [`sysenter` and `syscall`](#sysenter-and-syscall)

## `sysenter` and `syscall`
You use `sysenter` to expose a specific kernel module to the program, and `syscall` to call a specific function from the exposed module.

```asm
.start
    mov fdx, 1 ; fdx = 1
    sysenter "ios" ; enter the module used for text input-output, "ios" = input-output-string
    syscall ; call function 1, which is `sys_ios_write`
```