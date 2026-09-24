<div align="center">
    <img height="300" src="https://github.com/bracesoftware/newasm/blob/main/readme_resources/newasm_logo.png">
</div>

# `NewASM` Release Notes
Welcome to **`NewASM`**: a low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

* Version: `build 34`
* Version of the runtime: `18`
* Kernel version: `10`

> [!NOTE]
> This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

> [!WARNING]
> Some unstable builds can eventually gain runtime and kernel updates. NewASM Runtime is a back-end collection of runtime engines that let the front-end systems work. We recommend immediatelly updating as soon as the runtime updates for a specific version. Same goes with the kernel.

## What's new or changed?

+ Added the new `list`s. A list is a non-continuous list of primitive objects: integers, floats, characters and strings. It is basically an equivalent of a C++'s standard vector. To declare a list, use the following syntax:

```asm
.data
    list ListName : intg ; or float, char, and string
```

To resize or access a specific index of the list, you have to use `resize` and `lea` instructions, respectively.

```asm
.text
    fetch ListName
    resize this, 10 ; make it 10 elements
    lea this, 3 ; load the third element

    mov this, 3
    mov tlr, *this
```

After using `lea`, the list will return the value stored on index 3. Attempting to modify a list without using `lea` first will cause a crash. After each resize, the index will be also invalidated.

+ Added new abstract procedures! Abstract procedures are procedures with no declared body.

```asm
.start
    [abstract] proc myabstractproctestLOL ; creates a proc object
    
    call myabstractproctestLOL  ; throws a recoverable exception, program continues
    
    [impl] proc myabstractproctestLOL ; we get the implementation
        mov tlr, "hello guyz"
        [native] call print
        halt 0
    end
    
    call myabstractproctestLOL ; now it does what it was implemented with
```

+ Added new artifacts! Artifacts are loadable containers containing compiled code. To make it easier to understand, they behave like Java's class files.
To create an artifact, use the artifact instruction:

```asm
.start
    artifact MyArtifact -> { ;this creates a file MyArtifact.asmartifact
        [force] .start ; tell the compiler optimizer to ignore the duplicate .start section
        proc ArtifactProcedure ; now equivalent to java class methods
            mov tlr, "Hello from the artifact"
            [native] call print
            halt 0
        end
    }
```

Now, to use an artifact, we have to load it into a program, as this thing above just creates an artifact binary next to your app binary; so to create this compiled artifact, you have to run your program.

### Forking the artifact
To load an artifact, use `fork`:

```asm
; completely different program
.start
    fork MyArtifact ; has to match the name MyArtifact.asmartifact

```

If the artifact loaded with no exceptions, we can simply call its procedures using the `home` pointer or by directly addressing the procedure with its home artifact:

```asm
    [home] call ArtifactProcedure ; home ptr must be set by fork instruction, fork is like fetch but for artifacts
    ; or java-style
    call MyArtifact.ArtifactProcedure
```

### Mixins
To make your program append or rather inject more code into a specific artifact, you use the `mixin` instruction.

Let's say an artifact declares an abstract procedure, only way to modify it is to access its object inside an artifact, so we use a mixin to implement it:

```asm
.start
    artifact TestArtifact -> {
        [force] .start
            proc TestArtifactMethod
                mov tlr, "ey"
                [native] call print
                halt 0
            end

            [abstract] proc GottaImplementThis
    }

    mixin TestArtifact -> {
        [impl] proc GottaImplementThis
            mov tlr, "oyy mixin impl works"
            [native] call print
            halt 0
        end
    }

    ; we can append bytecode to the same mixin infinitely
    mixin TestArtifact -> {
        mov tlr, "TestArtifact mixin by dentist loaded"
        [native] call print
    }
```

## Fixed issues

- No issues were reported.

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