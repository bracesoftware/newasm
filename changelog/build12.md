# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

* Version: `build 12`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

## What's new

+ Added the new contexts! Context is a data container, or rather its own type, that can hold different information linked to a specific textual key, which is very similar to hashmaps.

To declare a context, do:

```asm
.data
    ctx myContext: () ; nothing in it
```

You can initialize it while declaring it:

```asm
.data
    ctx myContext: ("key": some_data) ; some_data can be an integer, a float, a string or a character
```

To add new keys, or modify existing ones, use the NEW `merge` instruction:

```asm
.start
    merge &myContext, ("key": 1, "key2": 73.3) ; this instruction will modify `key` and add `key2`
```

If we want to delete `key`, we assign it the `nil` value:

```asm
.start
    merge &myContext, ("key": nil) ; `key` is now unavailable
```

To erase the context completely, and give it new keys, use `mov`:

```asm
.start
    mov &myContext, () ; deletes everything
    mov &myContext, ("lmao": something) ; deletes everything AND adds the new key
```

To access the value of a specific key, do:

```asm
.start
    mov tlr, myContext("key") ; we're indexing it like tuples, but instead of number indexes, we use the key name, just like hashmaps
```

## What's changed

- No important changes.

## Fixed issues

+ Fixed the tuples not being able to process strings correctly under certain circumstances.

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