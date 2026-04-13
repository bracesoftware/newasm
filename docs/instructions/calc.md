## Calculated calls and returns

> [!WARNING]
> This feature was added in build 29.

This little article covers usage of following instructions:
1. `callc`
2. `retc`

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

This way of declaring functions, or rather treating labels as functions, is way more efficient, and it also supports recursion, allowing complex recursive operations.

A simple example would be:

```asm
.start
    mov rax, 5 ; forward the argument
    callc Loop ; call the function
    ret 0 ; end the program
; function declaration
:Loop
    cmp rax, 0
    jz endLoop
    dec rax
    mov tlr, "Hello from Loop"
    call std::ios::writeln
    callc Loop ; we got recursion in newasm finally!
:endLoop
    retc