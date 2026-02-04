## Interoperability with compiled languages
NewASM is interoperable with C/C++ and other compiled languages.
Just make sure your DLL/SO follows this convention:

```cpp
/*
    NewASM Extension Development Kit
*/

#define _NEWASM_syscall_export extern "C" __declspec(dllexport) char*
#define _NEWASM_syscall(syscallid) __newasm_syscall_##syscallid
```
> (this is the whole SDK! xD)

You can then easily call your library like this:

```asm
.start
    mov dlx, "testlib"  ; you don't need to provide if it is DLL/SO
                        ; to keep code independent of platform
    mov fdx, 1
    sysenter "ext"
    syscall
```