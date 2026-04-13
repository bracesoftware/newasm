## Calculated calls and returns

> [!WARNING]
> This feature was added in build 29.

```asm
.start
    jmp main
:label
    mov tlr, "Hello from func"
    call std::ios::writeln
    retc
:main
    callc label
    mov tlr, "Hello from proc"
    call std::ios::writeln
    ret 0
```