<div align="center">
    <img height="300" src="https://github.com/bracesoftware/newasm/blob/main/readme_resources/newasm_logo.png">
</div>

# `NewASM` Release Notes
Welcome to **`NewASM`**: a low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

* Version: `build 31`
* Version of the runtime: `15`
* Kernel version: `10`

> [!NOTE]
> This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

> [!WARNING]
> Some unstable builds can eventually gain runtime and kernel updates. NewASM Runtime is a back-end collection of runtime engines that let the front-end systems work. We recommend immediatelly updating as soon as the runtime updates for a specific version. Same goes with the kernel.

## What's new or changed?

+ Anonymous functions have been severely improved regarding speed and functionality. Now you can use `callc`, `retc`, `loop` and such!
```asm
.text
    thread thisisfun -> {
        mov tlr, "this is really fun"
        call std::ios::writeln
        mov tlr, 908
        call std::ios::writeln
        mov tlr, 243.4
        call std::ios::writeln

        mov tlr, (proc)
            int 0x3
            mov rax, 4
            mov tlr, "Hi from lambda in thread\n"
            mov fdx, 1
            sysenter "ios"
            syscall
            {:lmao}
            syscall
            loop rax, lmao
            int 0x3
            halt 0
        (end)
    }
```
As you can see, lambdas in threads now have their own labels because the JIT compiler ignores the sealed labels within the `(proc)` block.

Another stupid example would be:
```asm
.text
	mov tlr, (proc)
		jmp label2
		{:label1}
		jmp label3
		{:label2}
		jmp label1
		{:label3}
		halt 0
	(end)
```

+ The NewASM compiler now allows following compile-time instructions for implementing very simple logic:
1. `ifdef`: checks if a symbol/flag is defined;
2. `ifndef`: checks if a symbol/flag is not defined;
3. `fi`: used for ending an `if` block. 

If you want to combine if-statements, just nest them, you don't need to use `fi` more than once.

```asm
def SMTH, 0
ifdef SMTH
	ifndef SMTH_ELSE
	; do something
fi
```

There are no `else` variants, you have to end each if-block with `fi`.

+ Added a new concept of address fetching - a `fetch` instruction! This instruction is used to fetch a procedure or variable pointer, so we can use it with the `this` keyword. This is very important when optimizing your code because the compiler then can generate code that doesn't do any dictionary lookup.

```asm
.data
    ./data
        intg variable: 4
    ./!data
.text
    fetch data::variable ; get variable's address
    mov this, 6 ; give it a new value
    mov tlr, *this ; dereference the this ptr
    call std::ios::writeln ; prints 6
```

As you can see, we look for the variable only once, and then use the pointer, which means we get around 1.75x faster than using the variable's name twice.

We can also fetch addresses of procedures:

```asm
.text
    fetch std::ios::writeln
    mov tlr, "Hello"
    call this ; prints Hello
    
    mov tlr, 465
    call this
    ; .. etc
    ; this is way faster than letting the dispatcher
    ; look for the procedure every time you use call
    ; callc is still faster tho
```

If you don't want a variable to be fetched, use the new `@safe` attribute.
```asm
.data
    @safe
    intg var: 0
.text
    fetch var ; segmentation fault
```

You can also fetch a context or a tuple and use `this` as a handle:

```asm
.text
    fetch tuplename
    mov rax, this(1) ; get second element from the tuple
    lea this, 4 ; load effective address for that tuple when moving at specific index

    fetch ctxname
    merge this, () ; delete all ctxname contents
```

The `this` pointer can be used on `del`, `movasx` and `movaddr` as well.

The `this` pointer can also be used as an operand by the address-of operator:
```asm
mov tlr, #this ; returns a safe pointer that doesn't have any access to your computer
```

+ You can enable exception source information logging when a program crashes.
```asm
.start
    using "cfg"
    sysenter "cfg"
    mov rax, 1
    mov fdx, 1
    syscall ; now you will see exactly what module of the vm's source code caused the newasm script to crash
```

## Fixed issues

+ Fixed issue #32.

## Important notes

+ Build 31 of the virtual machine is compiled using C++26.

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