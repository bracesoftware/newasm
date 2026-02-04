## Interoperability with compiled languages
NewASM is interoperable with C/C++ and other compiled languages.
Just make sure your DLL/SO follows this convention:

```cpp
/*
    NewASM Extension Development Kit
*/

#define _NEWASM_syscall_export extern "C" __declspec(dllexport) const char*
#define _NEWASM_syscall(syscallid) __newasm_syscall_##syscallid
```

> [!TIP]
> This is the whole SDK! ***xD***

You can then easily call your library like this:

```asm
.start
    mov dlx, "testlib"  ; you don't need to provide if it is DLL/SO
                        ; to keep code independent of platform
    mov fdx, 1
    sysenter "ext"
    syscall
```

### Example library
Below is an example of the `testlib` library on Windows, written in C++:

`testlib.cpp`:
```cpp
// hellodll.cpp
#include <iostream>
#include <string>

#define _NEWASM_syscall_export extern "C" __declspec(dllexport) const char*
#define _NEWASM_syscall(syscallid) __newasm_syscall_##syscallid


_NEWASM_syscall_export _NEWASM_syscall(1)() // we can easily assign a specific fdx, in this case 1
{
    std::cout << "Hello from DLL!" << std::endl; // we can use all standard C++ functions
    return "ayee string vracen!!"; // we return the string, this string will be put into tlr register
                                    // we must note that the string HAS to be static, or else the VM won't be able to reach the data
                                    // so for now this is useless for functions returning anything except status flags, "0", "1", etc
}

```

Compile it with:
```
path/to/g++ -shared -o testlib.dll testlib.cpp -Wl,--out-implib,libtestlib.a
```