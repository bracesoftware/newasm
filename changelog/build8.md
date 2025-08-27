# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

- Version: `build 8`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

## What's new

* Added the new `printenv` shell command.

* Added the new `"math"` kernel module:
```asm
using "math"
```
Provides the following functions:
| Module | ID    | Arguments | Description |
| ----- | ---------------- | --------- | ----------- |
| `math` | `1` | `tlr` | Finds `sin()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `2` | `tlr` | Finds `cos()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `3` | `tlr` | Finds `tan()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `4` | `tlr` | Finds `sqrt()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `5` | `tlr` | Finds `abs()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `6` | `tlr` | Finds `cbrt()` of the number inside `tlr`. Stores the result inside `tlr`. |

## What's changed
* Optimizations.
* Now the `.$using` is a compile-time instruction, so instead of typing `.$using "ios"` you just do:
```asm
using "ios" ; or any other
```

## Fixed issues

- We've been focused on code optimisation.

## Important notes

- No important notes.

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