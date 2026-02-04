/*
    NewASM Extension Development Kit
*/

#define _NEWASM_syscall_export extern "C" __declspec(dllexport) const char*
#define _NEWASM_syscall(syscallid) __newasm_syscall_##syscallid
