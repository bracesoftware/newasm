# Execution flow instructions
These instructions define how your program behaves under certain circumstances.
Following instructions are covered by this article:
1. [`ret` and its variants](#ret-and-its-variants)
2. [`proc`, `end` and `call`](#proc-end-and-call)
3. [`cmp`, `jmp` and other `jmp` variants](#cmp-jmp-and-other-jmp-variants)
4. [`evt`](#evt)

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

## `proc`, `end` and `call`
These are used to declare a function/procedure and call it.

```asm
.start
    proc myproc
        ; do smth
        halt 0 ; return 0
    end

    call myproc ; calls the function
```

## `cmp`, `jmp` and other `jmp` variants

Compare register values with lvalues and jump to a specific label according to the result.

```asm
    cmp tlr, 2
    jz label ; if tlr == 2 jump to label
```

| Instruction | Processed if... |
| ----------- | --------------- |
| `je`/`jz` | comparsion returned `equal`. |
| `jne`/`jnz` | comparsion returned `not equal`. |
| `jl` | comparsion returned `less`. |
| `jg` | comparsion returned `greater`. |
| `jle` | comparsion returned `less or equal`. |
| `jge` | comparsion returned `greater or equal`. |

The `jmp` instruction is used as an unconditional jump.

## `evt`
Allows you to have events, read more about this instruction inside the concepts docs.