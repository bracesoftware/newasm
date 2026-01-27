# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

* Version: `build 12`
* Version of the runtime: `3`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

*ALERT!*: Some unstable builds can eventually gain runtime updates. NewASM Runtime is a back-end collection of backend engines that let the front-end systems work. We recommend immediatelly updating as soon as the runtime updates for a specific version.

## What's new

+ Updated the runtime so the profiling at the end of the program is more accurate.

## What's changed

- No important changes were made.

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

- If you are on Linux, just add the `.out` extension:

```bash
./newasm yourfile.asm
```

### Writing your first `NewASM` app

- Create the file named `yourfile.asm`, or just name it whatever you like, and edit it with an editor of your choice:

```asm
using "ios"
.data
    string text : "Hello world!"
    intg len : $-text
.start
    mov tlr, text
    mov stl, 0c1
    mov bos, len
    mov fdx, 1

    sysenter "ios"
    syscall
    ret 0
```

Output:
```
Hello world!
```