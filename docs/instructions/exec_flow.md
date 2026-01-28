# Execution flow instructions
These instructions define how your program behaves under certain circumstances.
Following instructions are covered by this article:
1. [`ret` and its variants](#ret-and-its-variants)

## `ret` and its variants
There are 4 variants:
1. `ret`: just ends the program with the specific exit code:
```asm
ret 0
```
2. `retn`: ends the program, but reads a register value for you; stands for "return-near":
```asm
mov tlr, 4
retn tlr ; ends the program with exit code 4
```
3. `retf`: used to end a specific thread, stands for "return-far".
```asm
.start
    thread threadName -> {
        ; do smth
        retf 0
        ; code under retf won't be executed
    }
```
4. `halt`: does the same thing as `ret`, but ends a procedure/function.
```asm
.start
    proc printText
        mov fdx, 1
        mov stl, 0c1
        sysenter "ios"
        syscall
        halt 0
    end
```