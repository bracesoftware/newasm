<div align="center">
    <img height="300" src="https://github.com/bracesoftware/newasm/blob/main/readme_resources/newasm_logo.png">
</div>

# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

* Version: `build 25`
* Version of the runtime: `10`
* Kernel version: `5`

> [!NOTE]
> This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

> [!WARNING]
> Some unstable builds can eventually gain runtime and kernel updates. NewASM Runtime is a back-end collection of runtime engines that let the front-end systems work. We recommend immediatelly updating as soon as the runtime updates for a specific version. Same goes with the kernel.

## What's new or changed?

+ Added the offline mode. Use `int 0x4` to enable it. The offline mode blocks system calls in TCP, HTTP and network module.
+ The `push` instruction now follows the `imm` convention. This change was made so it becomes faster. Use `mov imm, 1` when you are pushing a procedure onto the stack, and `zero imm` when you're pushing normal values.
+ More aggressive optimizations regarding the `evt` instruction.
+ Introduced the hybrid allocator in the RAM emulator.
+ Optimized the `int` instruction.
+ Optimized the `align` instruction.
+ Heavily optimized the runtime lvalue parsing, register dereferenciation and variable lookup.
+ Heavily optimized the `cmp` instruction (tests show `100ms` overhead fix!).
+ Optimized the standard library procedures (tests show they're `50ms` to `70ms` faster).
+ Added the `pragma` compile-time instruction that tells the compiler and the runtime how to behave at certain occurences.
+ Fixed crashing on label checking on compile-time.
+ Added a new `loop` instruction:
```asm
    mov tlr, "Hello world\n"
    mov rax, 4
    :test4
    call std::ios::write
    loop rax, test4
```
This code prints `Hello world` 4 times.
+ Added many new system calls regarding text operations. View more in `syscall` docs.
+ Added new `rbx` register that holds 32 bits, used as a float value.

## Fixed issues

- No issues were found.

## Important notes

- No important notes.

--------------------------------------------------------------------------------

### Building from source
- Use the following command to compile your own build of `NewASM`; make sure that you have `g++` and `go` installed:

```bash
go build networking.go -buildmode=c-archive -o networking.a
C:\path_to_your_compiler\g++ -static -std=c++23 index.cpp -o index.exe networking.a
```

- If you are using Windows Subsystem for Linux, use the following command:

```bash
wsl go build networking.go -buildmode=c-archive -o networking.a
wsl g++ -m64 -static -std=c++23 index.cpp -o index.out networking.a
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