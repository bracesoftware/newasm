# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

- Version: `build 6 RC 1`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

## What's new
* Added the new `tuple` datatype! Basically an array that can hold any data.
```asm
.$using %ios
.data
    tuple mytuple: () ; empty tuple
.start
    mov tlr, mytuble(0) ; access the tuple value at a specific index
    mov tlr, (43, 1.7, "Hi") ; or set `tlr` to be a tuple
```
Tuples can be only one-dimensional.

In order to update the specific index inside a tuple, you have to use the new `lea` instruction:
```asm
lea &tuple_name, 0 ; numeric index
mov tlr, some_value
stor tlr, &tuple_name ; just update the value at index 0
```
In order to completely change the tuple, just set the specific register to a tuple:
```asm
mov tlr, ("ey", 67, 1.2, 'a')
stor tlr, &tuple_name ; change the whole tuple
```
To expand the new tuple functionality, new kernel module has been introduced, `%tuple`.

| Module | ID | Arguments | Description |
| -------------|---------|------------|--------------- |
| `%tuple` | `1` | `tlr` | `tlr` is a pointer to a tuple. Stores the size of a tuple inside the `tlr` register. |


## What's changed
* Since we added tuples, also known as dynamic objects, standard object syntax has been changed to:
```asm
mov tlr, member @ object ; old syntax
mov tlr, object{member} ; new syntax to match tuple(index)
```
To change the value of a member inside an object, just add the ampersand:
```asm
stor tlr, &object{member}
```



## Fixed issues

- We've been focused on code optimisation.

## Important notes

* Tuple management can be only done with the `tlr` register.

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