# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

- Version: `build 9`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

## What's new

* Now in order to provide arguments to the NewASM virtual machine, you use the `newasm_args` environment variable:
1. on Windows:
```bat
set newasm_args = arg1, arg2
rem or
set newasm_args=arg1,arg2
rem syntax doesn't matter
```
2. on Linux:
```bash
export newasm_args=arg1,arg2
```

* To provide the app the VM is going to execute just use the normal arguments:
```
newasm.exe app.asm
```

* The new `nil` keyword is used for:
1. when you do not want to use the value you pop off the stack;
```
pop nil ; pops into nothin'
```
2. when you declare uninitialized references (lvalues);
```
.data
    ref lvalue: nil
```
3. when you want to free the last memory allocation.
```
malloc bytes
free nil ; instead of old "free"
```

* Added the following shell commands:
1. `passwd` - change your password;
2. `usernm` - change your username;
3. `renenv` - rename an environment variable.

## What's changed
* Now you can control what `malloc` you want to free:
```asm
.data
    intg alloc: 0
.start
    malloc 4
    mov &alloc, *tlr ; tlr register holds the address of the allocated memory
    ; some code
    free alloc ; free the memory you allocated
```

* Instead of:
```asm
halt proc, value
```
you just do:
```asm
halt value
```

## Fixed issues

* Now you can use `switch` and `case` statements within procedure blocks.

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