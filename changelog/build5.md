# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

- Version: `build 5`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

## What's new
* Added I/O ports for disk management.
* Added namespaces:
```asm
.data
    ./locals
        [lock]
        txt variable: "hi"
        [!lock]
    ./!locals
.start
    mov tlr, locals::variable
    mov stl, 0c1
    mov bos, 2
    mov fdx, 1
    sysenter %ios
    syscall
    ret 0
```


## What's changed
* The following instructions have had their deprecated suffix part removed: `push`, `pop`
* Removed the `-test` argument.


## Fixed issues
- No important fixes.
- We've been focused on code optimisation.

## Important notes

* Added support for older versions of Windows (Vista, 7, 8, 8.1).

--------------------------------------------------------------------------------

### Building from source
- Use the following command to compile your own build of `NewASM`; make sure that you have G++ installed:

```bash
C:\path_to_your_compiler\g++ -static -std=c++20 index.cpp -o index.exe
```

- If you are using Windows Subsystem for Linux, use the following command:

```bash
wsl g++ -m32 -static -std=c++20 index.cpp -o index.out
```

### Downloading
- Download one of the following archives that suits your system. Once you have downloaded it, extract the archive into a folder of your choice and begin using the application.

### Using the application
- Use the following command to execute your `NewASM` programs on Windows:

```bash
newasm -input yourfile.asm
```

- If you are on Linux, just add the `.out` extension:

```bash
./newasm.out -input yourfile.asm
```

### Writing your first `NewASM` app

- Create the file named `yourfile.asm`, or just name it whatever you like, and edit it with an editor of your choice:

```asm
.$using %ios
.data
    txt string : "Hello world!"
    num len : $-string
.start
    mov tlr, string
    mov stl, 0c1
    mov bos, len
    mov fdx, 1

    sysenter %ios
    syscall
    ret 0
```

Output:
```
Hello world!
```