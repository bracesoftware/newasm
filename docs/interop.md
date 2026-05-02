## Interoperability with compiled languages
NewASM is interoperable with C/C++ and other compiled languages.
Just make sure your DLL/SO follows this convention:

```cpp
// NewASM Extension Development Kit
// Copyright (c) Brace Software Co.

#include <iostream>
#include <string>
#include <cstring>

namespace newasm
{
    namespace Types
    {
        const int Integer = 1;
        const int Float = 2;
        const int String = 3;
        const int Character = 5;
    }
}
namespace NewASM = newasm;

#pragma pack(push, 1)
struct ___newasm_union_STRUCT final
{
    private:
    unsigned int type;
    int Int;
    float Float;
    char Char;
    char String[255];

    public:
    inline void setType(int t)
    {
        this->type = t;
    }
    inline void setInt(int v)
    {
        this->Int = v;
    }
    inline void setFloat(float v)
    {
        this->Float = v;
    }
    inline void setChar(char v)
    {
        this->Char = v;
    }
   
    inline void setString(std::string v)
    {
        strncpy(this->String, v.c_str(), sizeof(this->String) - 1);
        this->String[sizeof(this->String) - 1] = '\0';
    }
};
#pragma pack(pop)
#define _NEWASM_UNION                   ___newasm_union_STRUCT
#define _NEWASM_syscall_export          extern "C" __declspec(dllexport) _NEWASM_UNION
#define _NEWASM_syscall(syscallid)      __newasm_syscall_##syscallid
#define _NEWASM_event_export(name)      extern "C" __declspec(dllexport) void __newasm_event##name

#define _NEWASM_event_args              ()
#define _NEWASM_syscall_args            ()
```

> [!TIP]
> This is the whole SDK! ***xD***

### Example library
Below is an example of the `testlib` library on Windows, written in C++:

`testlib.cpp`:
```cpp
// Test library
#include "exdk/newasm.h"

_NEWASM_event_export(onload) _NEWASM_event_args
{
    std::cout << "testlib.dll loaded" << std::endl;
    return;
}

_NEWASM_event_export(onexit) _NEWASM_event_args
{
    std::cout << "testlib.dll unloaded" << std::endl;
    return;
}

_NEWASM_syscall_export _NEWASM_syscall(1) _NEWASM_syscall_args
{
    _NEWASM_UNION result;
    result.setType(NewASM::Types::String);
    result.setString("ayee string returned!!");
    std::cout << "Hello from DLL!" << std::endl;
    return result;
}
```

Compile it with:
```
path/to/g++ -shared -o testlib.dll testlib.cpp -Wl,--out-implib,libtestlib.a
```

You can then easily call your library like this:

```asm
extern "testlib" ; tell the compiler to link your app with our lib
.data
    union result: nil
.start
    fetch result ; set this ptr to &result
                 ; so we can take the return value
    movas string ; since we're returning a string in the example
    mov dlx, "testlib"  ; you don't need to provide if it is DLL/SO
                        ; to keep code independent of platform
    mov fdx, 1
    sysenter "ext"
    syscall


    ; to get the value, just do
    mov tlr, *this ; your string is stored into tlr
```