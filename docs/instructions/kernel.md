# Kernel instructions
These instructions are used to manipulate with the kernel and access its functions.
Following instructions will be covered in the article:
1. [`sysenter` and `syscall`](#sysenter-and-syscall)
2. [`int`](#int)
3. [`sysreq`](#sysreq)

## `sysenter` and `syscall`
You use `sysenter` to expose a specific kernel module to the program, and `syscall` to call a specific function from the exposed module.

```asm
.start
    mov fdx, 1 ; fdx = 1
    sysenter "ios" ; enter the module used for text input-output, "ios" = input-output-string
    syscall ; call function 1, which is `sys_ios_write`
```


#### `syscall` list
| Module | ID    | Arguments | Description |
| ----- | ---------------- | --------- | ----------- |
| `ios` | `1` | `tlr`, `bos` | Prints exclusively text with the size of `bos` (byte output size). Uses `stl` as a help argument. |
| `ios` | `2` | `tlr` | Prints integers and floating point numbers. Uses `stl` as a help argument. |
| `ios` | `3` | - | Requests textual user input and stores the value in `tlr`. |
| `ios` | `4` | - | Requests numeric (including floats) user input and stores the value in `tlr`. |
| `ios` | `5` | `tlr` | Prints the textual value of a built-in operand. |
| `ios` | `6` | `tlr` | Prints the name of a symbol a reference is pointing to. |
| `ios` | `7` | `tlr` | Prints a single character. |
| `ios` | `8` | - | Requests a single character input from the user and stores the value in `tlr`. |
| `fs/host` | `1` | `tlr` | Create a directory; with `tlr` being a string containing the directory name. |
| `fs/host` | `2` | `tlr` | Remove a directory; with `tlr` being a string containing the directory name. |
| `fs/host` | `3` | `tlr` | Create a file; with `tlr` being a string containing the file name. |
| `fs/host` | `4` | `tlr` | Remove a file; with `tlr` being a string containing the file name. |
| `fs/host` | `5` | `tlr`, `stl` | Overwrite file content; with `tlr` being a string containing the file name, and `stl` being a string containing the new content. |
| `fs/host` | `6` | `tlr`, `stl` | Append content to file; with `tlr` being a string containing the file name, and `stl` being a string containing the content to append. |
| `fs/host` | `7` | `tlr` | Remove all file content; with `tlr` being a string containing the file name. |
| `fs/host` | `8` | `tlr`, `stl` | Read a file line; with `tlr` being a string containing the file name, and `stl` being the line number. Read content is subsequently stored in `tlr`. |
| `cmanip` | `1` | `cpt` | Clear the bit array, with `cpt` being a pointer holding the address of your bit array. |
| `cmanip` | `2` | `cpt` | Flip the bit array, with `cpt` being a pointer holding the address of your bit array. |
| `cmanip` | `3` | `cpt` | Reverse the bit array, with `cpt` being a pointer holding the address of your bit array. |
| `cmanip` | `4` | `cpt`, `tlr`, `stl` | Set a value at a specific index, with `cpt` being a pointer holding the address of your bit array, `tlr` an index and `stl` either 0 or 1 as a value. |
| `cmanip` | `5` | `cpt`, `tlr` | Get value stored at the bit array, with `cpt` being a pointer holding the address of your bit array and `tlr` being the index. After the `syscall`, the function will store the value in `tlr`. |
| `cmanip` | `6` | `cpt`, `tlr`, `stl` | Set-at-parent-of function, with `cpt` being a pointer holding the address of your binary tree, `tlr` being the index and `stl` the value. |
| `cmanip` | `7` | `cpt`, `tlr`, `stl` | Set-at-right child-of function, with `cpt` being a pointer holding the address of your binary tree, `tlr` being the index and `stl` the value. |
| `cmanip` | `8` | `cpt`, `tlr`, `stl` | Set-at-left child-of function, with `cpt` being a pointer holding the address of your binary tree, `tlr` being the index and `stl` the value. |
| `cmanip` | `9` | `cpt`, `tlr`, `stl` | Get-at function, with `cpt` being a pointer holding the address of your binary tree and `tlr` being the index. After the `syscall`, the function will store the value in `tlr`.|
| `net` | `1` | `tlr`, `stl` | Download a file, with `tlr` being a link to the file and `stl` being the output destination. |
| `mem` | `1` | `tlr` | Gets memory usage in bytes and stores it inside the `rax` register. |
| `txtop` | `1` | `tlr`, `stl` | Concatenate 2 textual values, with these registers holding the two values. |
| `txtop` | `2` | `tlr` | Remove whitespaces from string ends. |
| `txtop` | `3` | `tlr` | Tokenizes the string stored inside the `tlr` register. |
| `txtop` | `4` | `tlr` | Formats the string stored inside the `tlr` register. |
| `txtop` | `5` | `tlr` | Turns a string stored in `tlr` into an integer and stores it in `rax`. |
| `txtop` | `6` | `tlr` | Turns a string stored in `tlr` into a float and stores it in `rbx`. |
| `txtop` | `7` | `tlr`, `rax` | Gets a character stored in string stored in `tlr` at index `rax` and stores it into `tlr`. |
| `chrono` | `1` | `/` | Gets the current year and stores it in `tlr`. |
| `chrono` | `2` | `/` | Gets the current month and stores it in `tlr`. |
| `chrono` | `3` | `/` | Gets the current day in a month and stores it in `tlr`. |
| `chrono` | `4` | `/` | Gets the current hour and stores it in `tlr`. |
| `chrono` | `5` | `/` | Gets the current minute and stores it in `tlr`. |
| `chrono` | `6` | `/` | Gets the current second and stores it in `tlr`. |
| `thread` | `1` | `tlr` | Prints the thread output, with `tlr` being a thread pointer. |
| `thread` | `2` | `tlr` | Gets the thread return and stores it in `tlr`, with `tlr` firstly being a thread pointer as well. |
| `ext` | `/` | `/` | Call a system call from a specific extension (dynamic library). |
| `tcp` | `1` | `tlr`, `stl` | `tlr` is the IP, and `stl` the data we are sending. |
| `tcp` | `2` | `tlr` | Receive data from the IP address stored inside `tlr`. |
| `http` | `1` | `tlr` | `tlr` is the address. Used to get HTTP data. |
| `http` | `2` | `tlr`, `stl` | Send data, with `tlr` being the URL and `stl` the data. |
| `math` | `1` | `tlr` | Finds `sin()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `2` | `tlr` | Finds `cos()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `3` | `tlr` | Finds `tan()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `4` | `tlr` | Finds `sqrt()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `5` | `tlr` | Finds `abs()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `math` | `6` | `tlr` | Finds `cbrt()` of the number inside `tlr`. Stores the result inside `tlr`. |
| `misc` | `1` | `tlr`, `stl` | Returns a random number, with `tlr` being the minimum, and `stl` being the maximum. |
| `crypto` | `1` | `tlr` | Performs SHA256 hash on a string stored in `tlr`. The result is stored in `tlr`. |
| `crypto` | `2` | `tlr`, `stl` | Performs XOR encryption on a string stored in `tlr`. The result is stored in `tlr`. Store a specific key into `stl`. |
| `crypto` | `3` | `tlr`, `stl` | Performs XOR decryption on a string stored in `tlr`. The result is stored in `tlr`. Store a specific key into `stl`. |
| `tuple` | `1` | `tlr` | Get tuple size, `tlr` is a reference to a tuple. Result is stored in `tlr`. |
| `ctx` | `1` | `tlr` | Get context size, `tlr` is a reference to a context. Result is stored in `tlr`. |
| `fs/vdsk` | `1` | `tlr`, `stl` | Create a file named `tlr`, with content in `stl`. |
| `fs/vdsk` | `2` | `tlr` | Remove a file named `tlr`. |
| `fs/vdsk` | `3` | `tlr` | Check if a file named `tlr` exists. |
| `fs/vdsk` | `4` | `tlr` | Create a directory or advance into that directory, go back with `..`. |
| `fs/vdsk` | `5` | `tlr` | Reads a file named in `tlr` and stores the data in `tlr`. |
| `fs/vdsk` | `6` | `tlr`, `stl` | Opens a file named in `tlr` and stores the data stored in `stl` into the file. |
| `fs/vdsk` | `7` | `tlr`, `stl` | Reads a file named in `tlr` and appends the data stored in `stl` into the file. |
| `fs/vdsk` | `8` | `tlr` | Reads a file named in `tlr` and prints the contents of the file. |

## `int`
This instruction is used to directly talk to the system.

```asm
int 0x3 ; tells the system to manually calculate the `bos` size for output
```

| System interrupt | Description |
| ---------------- | ----------- |
| `0x3` | Toggles the automatic `bos` calculation when printing text. |
| `0x4` | Toggles the offline mode. When offline mode is on, host service calls that require internet connection are blocked. |

## `sysreq`

Use this instruction to check if a symbol exists!

```asm
.data
    intg myData: 0
.start
    sysreq &myData -> intg ; checks if an integer named myData exists
```

Works also for procedures and threads.