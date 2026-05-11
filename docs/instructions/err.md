## Error handling

> [!WARNING]
> This feature was added in build 32.

This article covers the following instructions:
1. [`try`](#try-and-catch)
2. [`catch`](#try-and-catch)
3. [`throw`](#throw)

### `try` and `catch`
Instruction `try` is used to begin a try-catch block. Essentially, `try` tells the program termination system that it should expect a `catch` instruction. When your app encounters an error, the program temporarily stops and checks if `try` was used, if it was it is going to jump to nearby `catch` instruction and then jump to an error-handling label.

```asm
.text
    try
        fetch nil ; set this ptr to nothing
        mov this, 0 ; forcefully invoke an error
        ; <- since the error happened, code stops here and jumps to catch
        nop ; more random code
    catch error ; we provide a label
    :come_back

    ; more code

    ret 0 ; terminate your program

    :error
    mov tlr, "Error handled, error code: "
    call std::ios::write
    mov tlr, *rax ; catch ins stores an error code in rax
    call std::ios::writeln
    jmp come_back
```

You can use these in threads, procedures and lambda procedures. It is important to note you cannot have these nested at singular identation level, however you can have a separate try-catch block within a lambda and a thread.

So, you can't do:
```asm
try
    try
        try
        catch what
    catch smth
catch no
```

If you call a procedure that contains an error within try-catch block, and that procedure doesn't have its own catch, the procedure forcefully terminates and fallbacks onto the parent try-catch block.

So, you can do:

```asm
.text
    proc ProcWithError
        pddsfdssd ; purposefully try to use a non-existing dyn lib
        mov tlr, "hi from ProcWithError"
        sysenter "ios"
        mov fdx, 1
        syscall
        halt 0
    end

    try
        call ProcWithError
    catch ErrorHandler

    
    try
        fetch nil
        mov this, 3
        nop
    catch ErrorHandler
:Continue    
    zero rax
    mov rax, 223 ; exit code
    ret *rax ; returns from the whole program to the host OS or newasm shell

:ErrorHandler
    mov tlr, "Error properly catched and handled, exit code: "
    call std::ios::write
    mov tlr, *rax
    call std::ios::writeln
    jmp Continue
```

Output will be:
```
Error properly catched and handled, exit code: 10
```

Or you can modify the `ProcWithError` procedure to just ignore the error with `catch nil`:
```asm
.text
    proc ProcWithError
        try
            pddsfdssd ; purposefully try to use a non-existing dyn lib
        catch nil ; just continue with execution
        
        mov tlr, "hi from ProcWithError\n"
        sysenter "ios"
        mov fdx, 1
        syscall
        halt 0
    end
```

And you get the output:

```
hi from ProcWithError
Error properly catched and handled, exit code: 14
```

### Warnings
1. Compiler is going to give you an error if you try to nest these on the same identation level.
2. Compiler can optimize and remove empty try-catch blocks, such as:
```
try
catch smth
```
3. You are going to get a runtime error if you do a `callc` within a try-catch block if the address you are jumping to has a try-catch block.

### `throw`

> [!WARNING]
> This feature was added in build 33.

This instruction is used to manually invoke an error.

```asm
.text
    try
        throw
    catch nil
    ; ...
```