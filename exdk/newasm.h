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