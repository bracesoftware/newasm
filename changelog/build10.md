# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

* Version: `build 10`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

## What's new

+ Added the new `union`s! To declare a new union do this:
```asm
.data
    union myUnion: nil ; or anything else
```

Unions are semi-dynamic, which means you can assign any value type to them, but also you have to reallocate (free) memory every time you change their value:

```asm
; to set their value
movas intg ; since it is `nil` 
del &myUnion ; delete the memory

movas string ; new type
mov &myUnion, "Some text" ; now myUnion is a string "Some text", while it was just a number 0
```

+ Added the new `misc` kernel module with a new function:

| Module | ID    | Arguments | Description |
| ----- | ---------------- | --------- | ----------- |
| `misc` | `1` | `tlr`, `stl` | Returns a random number, with `tlr` being the minimum, and `stl` being the maximum. |


## What's changed

- No important changes.

## Fixed issues

- No important fixes.

## Important notes

+ When we declare an union as `nil` it is actually saved in memory as an integer with value of 0.

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