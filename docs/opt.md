## Compiler optimizations
The NewASM compiler does several different optimizations by default before packing your app into a binary file. Below is a list of optimizations compiler will do.

### Simple optimizations
The compiler will of course perform minor peephole optimizations such as:

1. `peephole optimization, removed redundant double code`

Code such as double jumps, double returns, or double reassignments, for example:

```asm
mov tlr, 3
mov tlr, 3 ; removed, it is already 3
```

```asm
jmp lmao
jmp smth ; removed, this line will never be reached
```

2. `removed redundant try-catch block`

Empty try-catch blocks will be ultimatively removed from the binary as they're relatively expensive.

```asm
try
; no code
catch xd
```

3. `peephole optimization, removed redundant refetch`

Double `fetch` instructions are removed.

4. `peephole optimization, removed redundant stack memory dedication`

Double `resb` instructions are removed.

5. `peephole optimization, removed redundant kernel module entrance`

Double `sysenter` instructions are removed.

6. `peephole optimization, removed redundant memory alignment`

Double `align` instructions are removed.

7. `peephole optimization, value of a register changed to itself`

Code such as:

```asm
mov tlr, *tlr
```

will also get removed by the compiler.

8. `peephole optimization, removed dead code`

Dead instructions like `rem` will get removed.

9. `peephole optimization, removed redundant assignment before reassignment`

Code like this will also be modified:
```asm
mov tlr, 6 ; removed cuz the next instruction's modifying tlr again
mov tlr, *tlr 
```

### More complex optimizations

1. `removed redundant code section reset`

Redundant code section modifiers are also removed.

```asm
.start
    ; code
.start ; removed
    ; more code
```

Although this looks like a very simple optimization, it requires some deeper code analysis by the compiler in order to determine if the removal is 100% safe.

2. `removed unreachable code block`

The compiler also does deeper code analysis in order to remove unreachable code blocks such as:

```asm
jmp lmao
    ; all of this code between there 2 lines will get removed, including the jmp ins above
    ; :lmao remains because the rest of the code may use it
:lmao

```

3. `removed useless namespace labels`

The compiler also removes empty namespaces. By empty, it means no procedure, thread, data container or a variable is declared within it.

```asm
.start
    ; empty namespace
    ./lmao
        nop
        mov rax, 0
        lea tuple, 3
        ; ... etc
    ./!lmao

    ; namespace with elements
    ./lmao
        proc func
            halt 0
        end

        ; it is gonna detect empty nested namespaces
        ; the compiler creates a complex tree of namespaces
        ; analyses it from top to bottom
        ./lmao2
        ./!lmao3
    ./!lmao
```

