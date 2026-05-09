<div align="center">
    <img height="300" src="https://github.com/bracesoftware/newasm/blob/main/readme_resources/newasm_logo.png">
</div>

# `NewASM` Release Notes
Welcome to **`NewASM`**: a low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

* Version: `build 32`
* Version of the runtime: `16`
* Kernel version: `10`

> [!NOTE]
> This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

> [!WARNING]
> Some unstable builds can eventually gain runtime and kernel updates. NewASM Runtime is a back-end collection of runtime engines that let the front-end systems work. We recommend immediatelly updating as soon as the runtime updates for a specific version. Same goes with the kernel.

## What's new or changed?

+ Added the new `resb` instruction.
You use this instruction to ask the VM to dedicate `n`-number of bytes to a specific thread and use it as stack space.

```
.text
    thread myThread -> {
        resb 64 ; ask the system for 64 bytes of thread-safe stack space
        push 429
        push 'c'
        push 873.45
    }
```

Now, 64 bytes of stack space we got from the VM is just a part of the heap, but it is internally used as stack space since each thread has its own copy of the stack pointer because of the context-switching.

You can also use the `stack` instruction, the JIT compiler replaces standard `push`, `pop` and `stack` with internal thread-specialized versions. That means you can call procedures that accept arguments. This approach ensures memory thread-safety and speed.

You can still have access to the global stack from within the child procedures and encapsulated lambda procedures.

+ Added instructions for error handling, `try` and `catch`.

```asm
try
nop ; some code
catch ErrorLabel ; if there was an error, in code above, then jump to label
:ErrorLabel
; code that exits the app normally
; rax is gonna contain error code that was supposed to crash the application
```

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

+ Optimized the following instructions:
1. `heap`
2. `db`
3. `free`

## Fixed issues

+ Fixed issue #36.
+ Fixed issue #38.
+ Fixed the `loop` instruction going into an infinite loop when the register is below 0 or 0.

## Important notes

- No important notes.

--------------------------------------------------------------------------------

### Building from source
- Use the following command to compile your own build of `NewASM`; make sure that you have `g++` and `go` installed:

```bash
go build networking.go -buildmode=c-archive -o networking.a
C:\path_to_your_compiler\g++ -static -std=c++26 index.cpp -o index.exe networking.a
```

- If you are using Windows Subsystem for Linux, use the following command:

```bash
wsl go build networking.go -buildmode=c-archive -o networking.a
wsl g++ -m64 -static -std=c++26 index.cpp -o index.out networking.a
```

### Downloading
- Download one of the following archives that suits your system. Once you have downloaded it, extract the archive into a folder of your choice and begin using the application.

### Using the application
- Use the following command to execute your `NewASM` programs on Windows:

```bash
newasm yourfile.asm
```

- If you are on Linux, just add the `./` suffix:

```bash
./newasm yourfile.asm
```

### Writing your first `NewASM` app

- Create the file named `yourfile.asm`, or just name it whatever you like, and edit it with an editor of your choice:

```asm
using "ios"
.data
    string text : "Hello world!\n"
    intg len : $-text
.start
    mov tlr, text
    mov bos, len
    mov fdx, 1

    sysenter "ios" ; entering the kernel
    syscall ; calling the system call/host service
    ret 0
```

Output:
```
Hello world!
```