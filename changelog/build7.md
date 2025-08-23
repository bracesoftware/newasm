# `NewASM` Release Notes
Welcome to **`NewASM`**: an interpreted low-level programming language which combines explicit memory and register control, giving it a breeze of assembly-like feel, with high-level functionalities such as objects, threads and more.

- Version: `build 7`

*NOTE*: This is a pre-release which means that this product version doesn't represent the final quality of the product - it may contain bugs and problems that aren't yet discovered.

## What's new
* Since variables are now objects that point to a specific memory, `movaddr` instruction has been introduced. The named instruction changes the address of a specific symbol.
```asm
.data
num number: 0
.start
del &number
malloc 4
mov hea, [0]
load *, 9938 ; some number
movaddr &number, *hea
free
```
* Instead of using `%`-prefixed hardcoded constants when using a kernel module, we just do:
```asm
.$using “ios”
.start
  mov tlr, “Hi”
  mov bos, 2
  mov fdx, 1
  mov stl, 0c1
  sysenter “ios”
  syscall
```
* Added the `tcp` kernel module used for basic TC protocol manipulation.
| Module | `fdx` | Arguments | Description |
| ---------------------- | ---------------------- | ---------------------- | ---------------------- |
| `tcp` | 1 | `tlr`, `stl` | `tlr` is the IP, and `stl` the data we are sending. |
| `tcp` | 2 | `tlr` | Receive data from the IP address stored inside `tlr`. |

* Added the address-of operator `#`:
```asm
mov tlr, # namespace::symbol_name
```
* Added the `http` kernel module used for basic HTTP protocol manipulation.
| Module | `fdx` | Arguments | Description |
| ---------------------- | ---------------------- | ---------------------- | ---------------------- |
| `http` | 1 | `tlr` | `tlr` is the address. Used to get HTTP data. |
| `http` | 2 | `tlr`, `stl` | Send data, with `tlr` being the URL and `stl` the data. |



## What's changed
* The whole virtual machine memory system has been completely rewritten to a more low-level model.
* Changed some of the datatype names:
`intg` instead of `num`;
`float` instead of `decm`;
`string` instead of `txt`.

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
.$using "ios"
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