<div align:"center">
    <img height:"200" src:"readme_resources/newasm_logo.png">
</div>

<h1 align : "center">Welcome to the <i><b>New-ASM</b></i> Wiki</h1>
<h3 align : "center">Interpreted Low-Level Language that Mimics Assembly</h3>

<div align:"center" style:"border-radius: 5px;">
    <img height:"30" align:"center" src:"readme_resources/brace_product.png">
    <img height:"30" align:"center" src:"readme_resources/cpp_logo.png">
</div>

# Documentation
Below is the simple `Hello World` program written in New-Assembly.

```asm
.data
    txt string : "Hello world"
.start
    mov tlr , string
    mov fdx , 1

    sysenter %ios
    
    syscall

    retn 0
```

# Table of contents
Documentation about `newasm` which includes following topics:
- [Compiling binaries](#compiling)
- [Arguments](#arguments)
- [Launch modes](#launch-modes)
    - [CTL mode](#ctl-mode)
        - [`install` command](#install-command)
- [Sections](#sections)
    - [`hndl` section](#hndl-section)
        - [Available events](#available-events)
    - [`data` section](#data-section)
    - [`start` section](#start-section)
    - [`text` section](#text-section)
- [Built-in references](#built-in-references)
- [Instructions](#instructions)
    - [`retn` and `ret`](#retn-and-ret-instructions)
    - [`mov` and `stor`](#mov-and-stor-instructions)
        - [Register list](#available-register-list)
    - [`syscall` and `sysenter`](#syscall-and-sysenter-instructions)
        - [System call list](#syscall-list)
    - [`int`](#int-instruction)
        - [System interrupt list](#list-of-system-interrupts)
    - [`nop`](#nop-instruction)
    - [`rem`](#rem-instruction)
    - [`sysreq`](#sysreq-instruction)
    - [`halt`](#halt-instruction)
    - [`push` and `pop`](#push-and-pop-instruction)
    - [`zero`](#zero-instruction)
    - [`heap`](#heap-instruction)
    - [`load`](#load-instruction)
    - [`jmp` and labels](#labels)
    - [`cmp` and `jmp` variants](#cmp-instruction-and-jmp-variants)
    - [Math calculations](#calculation-instructions)
    - [Bit operations](#bit-operations)
    - [`inc` and `dec` instructions](#inc-and-dec-instructions)
    - [`db` instruction](#db-instruction)
    - [`wait` instruction](#wait-instruction)
    - [`malloc` and `free` instructions](#malloc-and-free)
    - [`stack` instruction](#stack-instruction)
    - [`xchg` instruction](#xchg-instruction)
    - [`cls` instruction](#cls-instruction)
- [Procedures](#procedures)
- [Exit codes](#exit-codes)
- [Comments](#comments)
- [Interesting examples](#interesting-examples)
    - [Writing to a file, and then reading it](#writing-to-a-file-and-printing-its-content)
    - [Creating a child process](#creating-a-child-process)
- [Unassigned references](#unassigned-pointers-or-references)
- [Structures](#structures)
- [Project files](#project-files)
- [Dynamic libraries](#dynamic-libraries)
    - [Runtime errors](#runtime-errors)
- [Environment variables](#environment-variables)
- [Containers and data structures](#containers-and-data-structures)
    - [Bit arrays](#bit-arrays)
    - [Binary trees](#binary-trees)
- [Threads](#threads)
- [Opcodes](#instruction-set-and-opcodes)

## Compiling
This project is written purely in C++ using its standard libraries, so compiling it should be easy. To download C++ compiler, please follow instructions on the link below:

[Click me...](https://code.visualstudio.com/docs/languages/cpp)

## Arguments
- There are some arguments you can use when executing the interpreter.

| Argument | Parameters | Description |
| ---------------- | --------- | ----------- |
| `-ver` | - | Displays version information only, and doesn't start the interpreter at all. |
| `-help` | - | Displays help information. |
| `-input` | `<filename>` | Sets the input file; if `-input` was not provided, interpreter sets it to `input.asm`. |
| `-repl` | - | Enter the read-evaluate-print mode. |
| `-newproj` | - | Create a new project file if one doesn't already exist. |
| `-tests` | - | Start the test function. |
| `-log` | - | Toggle the logging system. |
| `-nover` | - | Turn off version checking. |
| `-mode` | `<id>` | Change the launch mode. |

## Launch modes
When running the `newasm` executeable, you can optionally use the `-mode` argument to open the application in different modes:
* mode 0: this is the default interpreter mode, it just does the primary idea of what it is supposed to do - run the assembly code;
* mode 1: this is the CTL, or control console, mode - application will run as the command prompt with its own commands, you can install packages and maintain your project.

### CTL mode
CTL mode brings new different commands with it. Below is a list of available commands:
| Command | Arguments | Description |
| ---------------- | --------- | ----------- |
| `help` | - | Displays this panel within the console. |
| `exit` | - | Closes the application. |
| `install` | `<lib>` | Install a library. |

#### `install` command
The install-command allows you to download and install New-ASM dynamic libraries and packages. There are 2 types of downloadable files - dynamic libraries (`.newasm_dl` files) and library setups (`.newasm_set` files). Setups allow library developers to provide more information about the library itself.

- An example library on the download server:
`internettest.newasm_dl` on `server.com/`:
```asm
mov fdx , 1
mov tlr , "Hi"
mov stl , %endl
sysenter %ios
syscall
zero stl
zero tlr
```

Example of command usage:
```cmd
install internettest
```
Notice how we just provided the library name, and not the extension. This will display the following information:

```
>>> install internettest
[NewASM]  PROGRAM THREAD @ System info | Attempting to install the "internettest" package.
        * Progress:        [::::::::::::::::::::::::::::::::::::::::] 100 %
                        Accessed the download server...
                        Successfully downloaded the library!
```

Now, if we want to download a package (or a setup), we use the same command, but the process will be different:
`net-conf.newasm_set` on `server.com/`:
```asm
~setup : msg
	Welcome to netconf!
~setup : dlname
	netconf
~setup : impl
	mov tlr , "INTERNET WORKS!"
	mov fdx , 1
	mov stl , %endl
    sysenter %ios
	syscall 
	zero stl
```
As you can see, we can use setup labels to mark code. Installing this with:
```cmd
install net-conf
```
we will receive the following output:
```
>>> install net-conf
[NewASM]  PROGRAM THREAD @ System info | Attempting to install the "net-conf" package.
        * Progress:        [::::::::::::::::::::::::::::::::::::::::] 100 %
                        Accessed the download server...
                        Successfully downloaded the setup.
                        Preparing to run the setup...
                        * Imported the dynamic library name...
                        * Imported the dynamic library install message...
                        * Imported the dynamic library implementation...
                        Successfully finished the setup.
                        Info: Welcome to netconf!
```


## Sections
Sections are built-in "tags" used to classify code. Each section uses different syntax in terms of instructions. General syntax is:

```asm
. section_name
```

### `hndl` section
In this section, you can setup event handlers.
```asm
. hndl
    ~exit , MY_EXIT_PROC
. start
    proc MY_EXIT_PROC
        ; code
        halt proc , 0
    end
    ; code
```

#### Available events
| Event name | Passed information | Description |
| ------------ | --------- | ----------- |
| `~exit` | / | Called when the program ends. |


### `data` section
In this section, you can declare variables to avoid repeated code. General syntax is:

```asm
data_type  variable_name : variable_value
```

You can use `variable_name` as an operand in instructions documented below.

There are 3 data types:
- `num` for integers;
- `decm` for floats;
- `txt` for strings;
- `ref` for references to data symbols (basically a kind of a pointer);
- `char` for characters.

### `start` section
In this section, you can perform instructions, and cannot create variables, or else program will end with exit code 1.

### `text` section
You can define macros here:

```asm
.text
    sayhi : #
        mov stl, %endl
        mov tlr, "hi from macro"
        mov fdx, 1
        sysenter %ios
        syscall
    #
.start
    $sayhi  ;test
```

Output:
```
hi from macro
```

## Built-in references
This language brings some built-in references, or rather operands, with itself - list:

- `%ios` - used as an operand in `syscall`, represents a module of system calls responsible for input and output streaming;
- `%fs` - used as an operand in `syscall`, represents a module of system calls responsible for input and output streaming;
- `%exf` - used as an operand in `syscall`, represents a module of system calls responsible for execution flow (starting child processes);
- `%cmanip` - used as an operand in `syscall`, represents a module of system calls responsible for container and data structure manipulation;
- `%net` - used as an operand in `syscall`, represents a module of system calls responsible for network features;
- `%mem` - used as an operand in `syscall`, represents a module of system calls responsible for memory/data management;
- `%endl` - line ending, used in `stl`;
- `&%null` - used to leave references/pointers unassigned/uninitialized.
- `%nl` - used as a null operand in some instructions.

**WARNING**: Syntax such as `% ios` is invalid.

## Instructions
Below is a list of available instructions. General syntax is:

```asm
instruction suffix , operand
```

### `retn` and `ret` instructions
Ends your program with specific exit code.

#### Example for `ret`
`ret` is a basic return, return a number or a variable value.

```asm
.start
    ret 23
```

Output:

```
[newasm] PROGRAM THREAD @ System info: Program finished with exit code : 23
```


#### Example for `retn`
`retn` stands for "return near" and you return register values.

```asm
. start
    mov tlr , 8
    retn tlr
```

Output:

```
[newasm] PROGRAM THREAD @ System info: Program finished with exit code : 8
```

### `mov` and `stor` instructions
Set value of a specific register.

#### Syntax for `mov`
- `instruction` - `mov`
- `suffix` - register name
- `operand` - new register value

#### Syntax for `stor`
- `instruction` - `stor`
- `suffix` - register name
- `operand` - variable name

#### Example `#1`
In this example, we basically do `fdx=1`:
```asm
. start
    mov fdx , 1
    retn 23
```

#### Example `#2`
In this example, we basically do `fdx=1`, `myvar=fdx`, `return 1`:
```asm
. data
    num  myvar : 0
. start
    mov fdx , 1
    stor fdx , myvar
    retn myvar
```

#### Available register list
- There are input and output registers. Input registers are used to get the input from the user, and output registers are used to store data which will be used as an argument in a `syscall` or an operand in an instruction.

| Register name | Full name | Description |
| ------------- | --------- | ----------- |
| `fdx` | function index | Holds an index of a function `syscall` will call. |
| `tlr` | typeless register | Typeless register (can hold any value, even code literals). Used as an output argument in some `syscall`s. |
| `stl` | secondary typeless register | Typeless register; however used to hold built-in operands for `syscall`s. |
| `stk` | stack pointer | Points at the top of the stack. |
| `hea` | heap pointer | Points at an address in the heap. |
| `psx` | procedure scope exit value | Holds value returned inside a procedure using `halt`. |
| `prp` | procedure pointer | Points at the procedure that was called using `call`. |
| `cpr` | comparsion result register | Holds a value of the comparsion (`cmp`) result; `1` for `equal`, `2` for less and `3` for greater (although there are 6 logical variants of `jmp`, only these 3 cases are required and detected by `cmp`). |
| `cr0` | primary calculation register | Register in which all the calculation results are stored. Read [this](#calculation-instructions) for more information... |
| `cr1` | alternate calculation register | Register which is used as a second operand in calculations. Read [this](#calculation-instructions) for more information... |
| `br0` | primary bit operation register | Register in which all the bitwise calculation results are stored. Read [this](#bit-operations) for more information...  |
| `br1` | alternate bit operation register | Register which is used as a second operand in bitwise calculations. Read [this](#bit-operations) for more information... |
| `cpt` | container pointer | Register holding an address specifically of a container or a data structure. Read [this](#containers-and-data-structures) for more information... |
| `tr0` | primary thread register | Dummy for threads. |
| `tr1` | alternate thread register | Another dummy for threads. |

### `syscall` and `sysenter` instructions
Perform a specific system call within a system module.

```asm
.start
    mov fdx , 1
    mov tlr , "Hello World"
    sysenter %ios ; enter the IO stream
    syscall ; do the call
    retn 23
```

#### `syscall` list
| Module | ID    | Arguments | Description |
| ----- | ---------------- | --------- | ----------- |
| `%ios` | `1` | `tlr`, `stl` | Prints exclusively text. Uses `stl` as a help argument. |
| `%ios` | `2` | `tlr`, `stl` | Prints integers and floating point numbers. Uses `stl` as a help argument. |
| `%ios` | `3` | - | Requests textual user input and stores the value in `tlr`. |
| `%ios` | `4` | - | Requests numeric (including floats) user input and stores the value in `tlr`. |
| `%ios` | `5` | `tlr` | Prints the textual value of a built-in operand. |
| `%ios` | `6` | `tlr`, `stl` | Prints the name of a symbol a reference is pointing to. |
| `%ios` | `7` | `tlr`, `stl` | Prints a single character. |
| `%ios` | `8` | - | Requests a single character input from the user and stores the value in `tlr`. |
| `%fs` | `1` | `tlr` | Create a directory; with `tlr` being a string containing the directory name. |
| `%fs` | `2` | `tlr` | Remove a directory; with `tlr` being a string containing the directory name. |
| `%fs` | `3` | `tlr` | Create a file; with `tlr` being a string containing the file name. |
| `%fs` | `4` | `tlr` | Remove a file; with `tlr` being a string containing the file name. |
| `%fs` | `5` | `tlr`, `stl` | Overwrite file content; with `tlr` being a string containing the file name, and `stl` being a string containing the new content. |
| `%fs` | `6` | `tlr`, `stl` | Append content to file; with `tlr` being a string containing the file name, and `stl` being a string containing the content to append. |
| `%fs` | `7` | `tlr` | Remove all file content; with `tlr` being a string containing the file name. |
| `%fs` | `8` | `tlr`, `stl` | Read a file line; with `tlr` being a string containing the file name, and `stl` being the line number. Read content is subsequently stored in `tlr`. |
| `%exf` | `1` | `tlr` | Start a new child process; with `tlr` being a string containing the file name. |
| `%cmanip` | `1` | `cpt` | Clear the bit array, with `cpt` being a pointer holding the address of your bit array. |
| `%cmanip` | `2` | `cpt` | Flip the bit array, with `cpt` being a pointer holding the address of your bit array. |
| `%cmanip` | `3` | `cpt` | Reverse the bit array, with `cpt` being a pointer holding the address of your bit array. |
| `%cmanip` | `4` | `cpt`, `tlr`, `stl` | Set a value at a specific index, with `cpt` being a pointer holding the address of your bit array, `tlr` an index and `stl` either 0 or 1 as a value. |
| `%cmanip` | `5` | `cpt`, `tlr` | Get value stored at the bit array, with `cpt` being a pointer holding the address of your bit array and `tlr` being the index. After the `syscall`, the function will store the value in `tlr`. |
| `%cmanip` | `6` | `cpt`, `tlr`, `stl` | Set-at-parent-of function, with `cpt` being a pointer holding the address of your binary tree, `tlr` being the index and `stl` the value. |
| `%cmanip` | `7` | `cpt`, `tlr`, `stl` | Set-at-right child-of function, with `cpt` being a pointer holding the address of your binary tree, `tlr` being the index and `stl` the value. |
| `%cmanip` | `8` | `cpt`, `tlr`, `stl` | Set-at-left child-of function, with `cpt` being a pointer holding the address of your binary tree, `tlr` being the index and `stl` the value. |
| `%cmanip` | `9` | `cpt`, `tlr`, `stl` | Get-at function, with `cpt` being a pointer holding the address of your binary tree and `tlr` being the index. After the `syscall`, the function will store the value in `tlr`.|
| `%net` | `1` | `tlr`, `stl` | Download a file, with `tlr` being a link to the file and `stl` being the output destination. |
| `%mem` | `1` | `tlr` | Marks a variable as a constant, with `tlr` being a pointer to the specific variable. Trying to modify a variable using `stor` afterwards will cause errors. |
| `%txtop` | `1` | `tlr`, `stl` | Concatenate 2 textual values, with these registers holding the two values. |
| `%txtop` | `2` | `tlr` | Remove whitespaces from string ends. |
| `%chrono` | `1` | `/` | Gets the current year and stores it in `tlr`. |
| `%chrono` | `2` | `/` | Gets the current month and stores it in `tlr`. |
| `%chrono` | `3` | `/` | Gets the current day in a month and stores it in `tlr`. |
| `%chrono` | `4` | `/` | Gets the current hour and stores it in `tlr`. |
| `%chrono` | `5` | `/` | Gets the current minute and stores it in `tlr`. |
| `%chrono` | `6` | `/` | Gets the current second and stores it in `tlr`. |


### `int` instruction
Send system interrupts, basically manipulate with the interpreter.
```asm
.start
    mov tlr, 36
    int 0x1 ; send a sys interrupt to modify memory size
```

#### List of system interrupts
| ID | Arguments | Description |
| ---------------- | --------- | ----------- |
| `0x1` | `tlr` | Modifies the size of a shared heap-stack memory block, with `tlr` holding the new memory size. |
| `0x2` | `tlr` | Tells the interpreter to skip checking for procedure validity when assigning event handlers with `tlr` being either 0 or 1 — off or on, respectively. |


### `nop` instruction
Do nothing.

#### Syntax
- `instruction` - `nop`
- `suffix` - no suffix
- `operand` - no operand

#### Example

```asm
. start
    nop
    retn 23
```

### `rem` instruction
Do nothing. NOT RECOMMENDED TO USE!

#### Syntax
- `instruction` - `rem`
- `suffix` - no suffix
- `operand` - your comment

#### Example

```asm
. start
    rem 0 , "my comment"
    retn 23
```

### `sysreq` instruction
Ensure that a symbol is available for further use.

#### Syntax
- `instruction` - `sysreq`
- `suffix` - `proc`, `data`
- `operand` - procedure return value

#### Example

```asm
. data
. start
    ; Notice how we haven't declared anything in .data
    sysreq data , variable
    ; Program will be terminated with exit code 4
    ; Same happens when we try to access a procedure:
    sysreq proc , some_random_proc
```

### `halt` instruction
Return a value inside a function.

#### Syntax
- `instruction` - `halt`
- `suffix` - `proc`
- `operand` - procedure return value

#### Example

```asm
. data
    num  variable : 0
. start
    proc 0 , testprocedure
        halt proc , 364
    end
    call 0 , testprocedure
    stor psx , variable
    mov tlr , variable
    mov fdx , 1
    sysenter %ios
    syscall
    retn 1
```

### `push` and `pop` instruction
- Push and pop values to and from the stack, respectively.

#### Syntax for `push`
- `instruction` - `push`
- `suffix` - no suffix
- `operand` - constant value or a value of a variable

#### Syntax for `pop`
- `instruction` - `pop`
- `suffix` - no suffix
- `operand` - variable to pop the value to

#### Example

```asm
. data
    num  myvar2 : 0
. start
    push 0 , 273


    ; change myvar2 to something dumb:
    stor fdx , myvar2


    pop 0 , myvar2

    mov tlr , myvar2
    mov fdx , 1
    sysenter %ios
    syscall

    retn 0
```

- **TIP**: If you just want to pop the value off the stack, and not store it anywhere, just do:

```asm
. start
    pop 0 , %nl
```

### `zero` instruction
Resets the register to an invalid value that cannot be used.

#### Syntax
- `instruction` - `zero`
- `suffix` - register name
- `operand` - no operand

#### Example

```asm
. start
    zero stl
    ; something ?
```

### `heap` instruction
Move down and up the heap.

#### Syntax
- `instruction` - `heap`
- `suffix` - no suffix
- `operand` - number of addresses to move up to

#### Example

```asm
. data
    num  mynum : 0
. start
    heap 0 , 3
    stor hea , mynum
    mov tlr , mynum
    mov fdx , 2
    sysenter %ios
syscall
```

### `load` instruction
- Load data and store data from the heap and into the heap, respectively.

#### Syntax
- `instruction` - `load`
- `suffix` - `adr`, `ref`
- `operand` - literal value or a reference

#### Example
```asm
. data
    decm  testdecimal : 0.0
. start
    ; If the suffix of the LOAD instruction is `adr`,
    ; then we will update the value in the address heap pointer
    ; is pointing to - HOWEVER, if the suffix is `ref`, then we will 
    ; store the value in the address heap pointer is pointing to
    ; into some variable in `.data`. 
    load adr , 736.38 ; hea : something
    load ref , testdecimal ; myvar : hea

    mov tlr , testdecimal
    mov stl , %endl
    mov fdx , 2
    sysenter %ios
    syscall

    retn 0
```

Output:

```
736.38
```

#### Example `#3`
- If we expand our code, and manually assign addresses before cleaning up the heap, we can do this:

```asm
. data
    decm  testdecimal : 0.0
    decm  testdecm2 : 0.0
. start
    load adr , 736.38 ; hea : something
    load ref , testdecimal ; myvar : hea

    mov tlr , testdecimal
    mov stl , %endl
    mov fdx , 2
    sysenter %ios
syscall

    ; Allocate more space:
    heap 0 , 1
    load adr , 9821.38 ; hea : smth
    load ref , testdecm2 ; myvar : hea
    mov tlr , testdecm2
    mov stl , %endl
    mov fdx , 2
    sysenter %ios
syscall
    
    mov hea , 0 ; manually access the first address
    load ref , testdecm2 ; myvar : hea
    mov tlr , testdecm2
    mov stl , %endl
    mov fdx , 2
    sysenter %ios
syscall

    mov hea , 1 ; manually access the second address
    load ref , testdecm2 ; myvar : hea
    mov tlr , testdecm2
    mov stl , %endl
    mov fdx , 2
    sysenter %ios
syscall

    heap 0 , -1 ; let all the memory go to avoid getting the memory leak
```

Output:

```
736.38
9821.38
736.38
9821.38
```

- **NOTE**: Procedures and variables you create in New-Assembly are not located in the stack and the heap of your program, so you have both the stack and the heap for yourself, which means that addresses in the heap start from 0 and go up by 1. You may ask, why is that - well New-Assembly mimics the assembly language and doesn't need to go by the rules set by the standards.

### Labels
- You can create labels inside `.start` and then jump to them using the `jmp` instruction. General syntax is:

```asm
. start
    : label_name

    ; somewhere
    jmp 0 , label_name
```

#### Example `#1`

```asm
. start
    : labelname
        mov . fdx , 4
        sysenter %ios
        syscall
    jmp 0 , labelname
```

#### Example `#2`
A little too complex example.
```asm
. start
    jmp 0 , label2
    : label
        mov tlr , "label called"
        mov fdx , 1
        sysenter %ios
syscall
        mov fdx , 72
        jmp 0 , label3
        ret fdx
    : label2
        mov tlr , "label2 called"
        mov fdx , 1
        sysenter %ios
syscall
        jmp 0 , label
    : label3
        mov tlr , "label3 called"
        mov fdx , 1
        sysenter %ios
syscall

    retn 3873
```

Output:

```
label2 called
label called
label3 called
[newasm] PROGRAM THREAD @ System info: Program finished with exit code : 3873
```

### `cmp` instruction and `jmp` variants
Compare values of registers with values stored in the operands.

```asm
cmp fdx , 3 ; check if fdx is 3
```

According to the result `cmp` stores in its own "hidden" register, you can use the following variants of the `jmp` instruction in order to perform jumps to labels according to the value of the `cpr` (comparsion result register):

| Instruction | Processed if... |
| ----------- | --------------- |
| `je` | comparsion returned `equal`. |
| `jne` | comparsion returned `not equal`. |
| `jl` | comparsion returned `less`. |
| `jg` | comparsion returned `greater`. |
| `jle` | comparsion returned `less or equal`. |
| `jge` | comparsion returned `greater or equal`. |

#### Example

```asm
. start
    mov tlr , 5.8
    cmp tlr , 1
    je 0 , equal
    ;jne 0, notequal
    jl 0 , less
    jg 0 , greater
    ;jle 0 , lesseq
    ;jge 0 , greatereq

    ; We just want to check if they are either equal, less or greater.

    : equal
        mov tlr , "EQUAL"
        mov fdx , 1
        mov stl , %endl
        sysenter %ios
syscall
        jmp 0 , endtheprogram

    : notequal
        mov tlr , "NOT EQUAL"
        mov fdx , 1
        mov stl , %endl
        sysenter %ios
syscall
        jmp 0 , endtheprogram

    : less
        mov tlr , "LESS"
        mov fdx , 1
        mov stl , %endl
        sysenter %ios
syscall
        jmp 0 , endtheprogram

    : greater
        mov tlr , "GREATER"
        mov fdx , 1
        mov stl , %endl
        sysenter %ios
syscall
        jmp 0 , endtheprogram

    : lesseq
        mov tlr , "LESS OR EQUAL"
        mov fdx , 1
        mov stl , %endl
        sysenter %ios
syscall
        jmp 0 , endtheprogram

    : greatereq
        mov tlr , "GREATER OR EQUAL"
        mov fdx , 1
        mov stl , %endl
        sysenter %ios
syscall
        jmp 0 , endtheprogram

    : endtheprogram
    retn 0
```

Output:
```
GREATER
```

### Calculation instructions
There are 6 mathematical instructions you can use:
- `add` - addition,
- `sub` - subtraction,
- `mul` - multiplication,
- `div` - division,
- `exp` - exponentiation,
- `log` - logarithm.

#### Syntax
All of these operations use `cr0` and `cr1` registers. The result is stored in `cr0`; so:

```asm
. start
    mov cr0 , 3.0 ; can be either a float or a whole number
    mov cr1 , 9 ; same

    add ; cr0 : cr0 + cr1
    sub ; cr0 : cr0 - cr1
    mul ; cr0 : cr0 * cr1
    div ; cr0 : cr0 / cr1
    exp ; cr0 : power(cr0,cr1)
    log ; cr0 : log_c1(cr0)
```

### Bit operations
There are 6 bitwise operations you can use:
- `and` - and,
- `or` - or,
- `not` - not,
- `xor` - exclusive or,
- `shl` - shift left,
- `shr` - shift right.

#### Syntax
All of these operations use `br0` and `br1` registers. The result is stored in `br0`; so:

```asm
. start
    mov br0 , 1 ; must be a number
    mov br1 , 1 ; same

    and ; br0 : br0 & br1
    or ; br0 : br0 | br1
    not ; br0 : ~br1
    xor ; br0 : br0 ^ br1
    shl ; br0 : br0 << br1
    shr ; br0 : br0 >> br1
```

### `inc` and `dec` instructions
These instructions are used to increment or decrement register values. Since registers can be of various types, these 2 instructions perform differently.

#### Syntax
```asm
inc register_name
dec register_name
```

#### Example `#1`
If you operate on a whole number or rational number register, then it may not be so interesting, it just increments it by 1.

```asm
inc br0 ; br0++
dec cr1 ; cr1--
```

If you are operating on a typeless register, these operations will check if the register is holding a whole number, and then apply changes to it. Namely, if you try to increment `tlr` while `tlr` is holding a string, you will get a data type mismatch exception.

However, what if you try to decrement a symbol reference, well - this gets interesting. For instance, if you tried to decrement the `prp` register (which is a reference to a last used procedure), you can't just decrement it by 1 since it is internally pointing to a pair in the procedure map, so the interpreter does the work for you and updates it to the pair before the pair `prp` is pointing at.

Demonstration:
```asm
. data
    ref  temporary : &%null
. start
    proc 0 , procedure_1
        halt proc , 0
    end
    proc 0 , procedure_2
        halt proc , 0
    end
    proc 0 , procedure_3
        halt proc , 0
    end

    mov prp , &procedure_1
    inc prp

    stor prp , temporary
    mov tlr , temporary
    mov stl , %endl
    mov fdx , 6
    sysenter %ios
syscall

    mov prp , &procedure_3
    dec prp

    stor prp , temporary
    mov tlr , temporary
    mov stl , %endl
    mov fdx , 6
    sysenter %ios
syscall
```

Output:

```asm
procedure_2
procedure_2
```

### `db` instruction
- A little instruction used to debug register values. Example:

```asm
. start
    ; ... some code
    db tlr
```

Output:
```
[NewASM]   PROGRAM THREAD @ Debug | tlr : `"string"`
```

### `wait` instruction
Basically pauses the program execution.
```asm
.start
    wait 2000 ; waits for 2000ms, or 2s
    retn 0 ; ends the program
```


### `malloc` and `free`
Easily manage heap memory. Example:

```asm
.start
    heap 40
    db hea
    malloc 2 ; for test purposes
    mov hea, 2
    db hea
    free
    db hea
    __say 0,"2nd malloc"
    malloc 2 ; allocate 2 more spaces in the heap
    mov hea, [1] ; using brackets we can access the address id of the malloc-allocated heap
    db hea
    free ; automatically get rid of that allocated memory
    db hea
    heap -40 ; free up the rest we occupied for some testing
```
Output:
```
[NewASM]   PROGRAM THREAD @ Debug | hea = `40`
[NewASM]   PROGRAM THREAD @ Debug | hea = `2`
[NewASM]   PROGRAM THREAD @ Debug | hea = `40`
                        2nd malloc
[NewASM]   PROGRAM THREAD @ Debug | hea = `42`
[NewASM]   PROGRAM THREAD @ Debug | hea = `40`
```

### `stack` instruction
Clear up the call stack.
```asm
.hndl
    0x827, procedurename ;assign a hex code to a procedure
.start
    proc procedurename
        mov tlr, <0> ;get the first argument
        mov stl, <1> ;get the second argument
        mov fdx, <2>
        sysenter %ios
        syscall
        halt proc, 0
    end
    db stk
    push 0, 1 ; push the third arg
    push 0, %endl ; push the second arg
    push 0, "call stack works" ; push the first arg
    push 0, 0x827 ; call the procedure
    stack ;clear up the stack after the procedure call
    retn 0
```
Output:
```
call stack works
```

### `xchg` instruction
This instruction exchanges the values of `tlr` and `stl`.
```asm
mov tlr, 1
mov stl, 2
xchg
db tlr
db stl
```
Output:
```
2
1
```

### `cls` instruction
Clears the screen.

## Procedures
Procedures allow you to use the same piece of code without having to actually repeat it. General syntax is:

```asm
proc procedure_name
    ; code
end
```

To call the procedure, use:

```asm
call procedure_name
```

- Here is an example:

```asm
. start
    proc test
        halt proc , 1
    end
    call test
    retn 0
```

Basically, these are just functions, but in assembly.

## Exit codes
When a fatal error happens, program will shut down, returning a specific exit code, below is a list of exit codes.

| Exit code | Description |
| ---------------- | ----------- |
| `0` | No termination point (standard exit code if you have no `ret` or `retn` in your script). |
| `1` | Invalid section (for example, tried to enter a section named `.lol`). |
| `2` | Attempted to call a procedure which does not exist. |
| `3` | Invalid non-numeric value was passed to `retn`. |
| `4` | `sysreq` failed, pretty self-explanatory. |
| `5` | Stack/heap collision; pretty self-explanatory. |
| `6` | Data overflow - tried to pop a value into unallocated address. |
| `7` | Data type mismatch. |
| `8` | Tried to redefine a label. |
| `9` | Bus error; tried to access an invalid label with `jmp`. |
| `10` | Invalid instruction; pretty self-explanatory, also happens if you use `.` or `,` (instruction parsing delimiters) in your `rem` comments. |
| `11` | Memory overflow. |
| `12` | Memory underflow. |
| `13` | Procedure redefinition. |
| `14` | Invalid memory access; tried to access an address using `mov.hea,...` that isn't allocated for the heap. |
| `15` | Invalid syntax. |
| `16` | Memory leak: neither `retn`, `ret` or `heap` were used at the end of the program while the `hea` register wasn't at 0. |
| `17` | Invalid configuration. |
| `18` | You tried to call a procedure within a procedure. |
| `19 `| Unknown system call index (invalid value moved into `fdx`). |
| `20 `| Tried to create a child process in a child process. |
| `21 `| Tried to jump to a label in a child process. |
| `22` | Attempted to use an unassigned pointer/reference variable. |
| `23` | Tried to redefine a variable. |
| `24` | Tried to redefine a structure. |
| `25` | Unexpected closing brace. |
| `26` | Tried to access a structure that hasn't been declared. |
| `27` | Tried to access an undefined structure member. |
| `28` | Tried to create a struct inside a struct. |
| `29` | Expected closing brace. |
| `30` | Tried to create an empty procedure. |
| `31` | Tried to redefine a data structure. |
| `32` | Unexpected usage of the `end` instruction. |
| `33` | Tried to use an improperly loaded dynamic library instruction. |
| `34` | Unknown event specified in the handler section. |
| `35` | Assigned an invalid procedure as an event handler. |
| `36` | Tried to modify a constant. |
| `37` | Operating system-related error. |
| `38` | Memory allocation error. |
| `39` | Tried to modify the memory manually before using `free`. |
| `40` | Invalid stack call. |
| `41` | Handle reassignment. |
| `42` | `sysenter` failed. |
| `43` | Macro redefinition. |
| `44` | Unexpected hash. |
| `45` | Undefined macro. |

## Comments
Comments are also available:

```asm
. start
    ; comment
    mov fdx , 1
    mov tlr , "hello" ; comment
    sysenter %ios
syscall ; comment again
```

## Interesting examples
Below is a list of interesting examples of using the language.
### Writing to a file, and printing its content

```asm
. start
    sysenter %fs
    mov fdx, 3
    mov tlr, "filename"
    syscall
    mov stl, "TEXTeee"
    mov fdx, 6
    syscall
    mov stl, 1
    mov fdx, 8
    syscall
    mov stl, %endl
    mov fdx, 1
    sysenter %ios
    syscall
    retn 0
```

Output:

```
TEXTeee
```

### Creating a child process
`index.nax`:
```asm
. start
    mov tlr, "child.nax" ; another nax file containing stuff such as config modifications, variables and procedures
    mov fdx, 1
    sysenter %exf
    syscall; create a process and execute it

    mov tlr , "Hi after the process" ; this line will be processed AFTER child.nax finishes executing
    mov stl , %endl
    mov fdx , 1
    sysenter %ios
syscall

    retn 0
```

`child.nax`:
```asm
. data
    num  mynum : 0
. start
    heap 36
    stor hea , mynum
    mov tlr , mynum
    mov fdx , 2
    sysenter %ios
syscall ; prints heap size (36)
    heap -36 ; free up memory we occupied for the sake of the example
```

Output:

```
36
Hi after the process
```

#### Notes regarding child processes
1. You cannot create labels and jump to them in child processes.
2. If you use `ret` or `retn` inside a child process, it will terminate the whole program with that exit code and not just the child process.
3. Procedures and variables created inside the child process can be used in the parent process (in our case `index.nax`) after the child process finishes executing.

## Unassigned pointers or references
```asm
.data
    ref  unassigned_pointer : &%null
```

This reference is left unassigned and an attempt to use it will result in an exception.

## Structures
In this quite frankly low-level language, there are also structs! General syntax is:
```asm
.data
    struct  structure_name : {
        ; members
        data_type  member_name : member_value
    }

```

### Example
```asm
. data
    struct  mystruct : {
        num  lol : 98
        decm  decimal : 2.3
        txt  text : "hi from struct"
    }
    struct  mystruct2 : {
        num  lol : 45
        decm  decimal : 833.4
        txt  text : "hi from struct again"
        ref  reference : &prptest
    }
. start
    mov tlr , text @ mystruct
    mov stl , %endl
    mov fdx , 1
    sysenter %ios
syscall

    mov tlr , decimal @ mystruct
    mov fdx , 2
    sysenter %ios
syscall

    mov tlr , lol @ mystruct
    mov fdx , 2
    sysenter %ios
syscall

    mov tlr , text @ mystruct2
    mov stl , %endl
    mov fdx , 1
    sysenter %ios
syscall

    mov tlr , decimal @ mystruct2
    mov fdx , 2
    sysenter %ios
syscall

    mov tlr , lol @ mystruct2
    mov fdx , 2
    sysenter %ios
syscall

    mov tlr , reference @ mystruct2
    mov fdx , 6
    sysenter %ios
syscall

    mov stl , 45657

    stor stl , lol @ mystruct2
    mov tlr , lol @ mystruct2
    mov fdx , 2
    mov stl , %endl
    sysenter %ios
syscall

    mov psx , "HIII243"

    stor psx , text @ mystruct
    mov tlr , text @ mystruct
    mov fdx , 1
    mov stl , %endl
    sysenter %ios
syscall
```

Output:

```
hi from struct
2.3
98
hi from struct again
833.4
45
prptest
45657
HIII243
```

### Notes
1. Currently, if you want to store a value into a struct member, only the `stor` instruction will work - note that instructions such as `load` and `pop` will not work regarding this.
2. This syntax is a must:
```asm
struct  name : { ; brace must be HERE
; and not here
    num  number : 384
}
```

## Project files
**Project files** (`.newasm_proj` file) are files that define information about your New-ASM project. General name format for them is:

```
<entry file name>.newasm_proj
```
For example, if my `-input` file is `input.asm`, project file for that would be `input.asm.newasm_proj`. This is basically an INI file.

### Keys and values available
| Key name | Description |
| -------- | ----------- |
| `name` | Name for your project. |
| `version` | Version of your project. |
| `dlibs` | Dynamic libraries your project is using. |

Example `index.asm.newasm_proj` file:
```ini
name : Unnamed project
version : 0.0.1
dlibs : testlib, sayhi
```

## Dynamic libraries
**Dynamic libraries** (`.newasm_dl` file) are files that provide user-made instructions. For example, let's say this is a NewASM dynamic library you wrote:

`sayhi.newasm_dl`:
```asm
mov tlr , "hi from my dynamic library"
mov fdx , 1
sysenter %ios
syscall
```

In your entry file - `index.asm`, you can do this:
```asm
. start
    mov tlr , "some stuff"
    inc prp ; bunch of operations
    ; more stuff...

    sayhi ; your very own custom instruction
```

This is literally a fancy way of making procedures, making them reusable across files, however there are limits:
1. You cannot create labels.
2. You thus cannot use jump instructions such as `jmp`, `je`, `jne` and more.

### Runtime errors
If you make an error inside a dynamic library, you will get a runtime error while the library was getting implemented. Below is a list of runtime errors that may occur:

| Runtime error code | Description |
| -------- | ----------- |
| `1` | Tried to create a label. |
| `2` | Tried to use an unsupported instruction. |

## Environment variables
Environment variables are typeless variables that are defined within the `.newasm/env_vars.ini` file. Let's take a look:

`.newasm/env_vars.ini`:
```ini
testenv = "hello from env var"
```

`index.asm`:
```asm
. start
    mov tlr , * / testenv
    mov fdx , 1
    sysenter %ios
syscall
```

* **NOTE**: You must use `*/` before the environment variable name so the program knows you are using an environment variable and not a standard variable, which means this wouldn't work:

```asm
. start
    mov tlr , testenv
    mov fdx , 1
    sysenter %ios
syscall
```

## Containers and data structures
- There are following data structures and containers available:
1. Bit arrays
2. Binary trees

### Bit arrays
- Arrays of bits. Internally, they are just integers, but you are provided `syscall`s to manipulate them as if they were really just arrays of bits, an example:

`index.asm`:
```asm
.data
    bit_arr  testbitarr : 0
.start
    sysenter %cmanip
    mov cpt , &testbitarr
    mov tlr , 2
    mov stl , 1
    mov fdx , 4
    syscall 
    mov tlr , 1
    mov fdx , 5
    syscall 
    mov fdx , 2
    mov stl , %endl
    sysenter %ios
    syscall
    mov tlr , 2
    mov fdx , 5
    sysenter %cmanip
    syscall 
    mov fdx , 2
    sysenter %ios
    syscall
```
Output:
```
0
1
```

* NOTE: To really understand what's going on, check out the list of available system calls.

### Binary trees
- Binary trees are specific arrays containing the parent nodes which have their right and their left child. Example:

`index.asm`:
```asm

.data
    bin_tree  testbintree : 0
.start
    mov cpt , &testbintree
    sysenter %cmanip
    mov tlr , 0
    mov stl , 33
    mov fdx , 7
    syscall 
    
    mov tlr , 0
    mov fdx , 9
    syscall 

    mov stl , %endl
    mov fdx , 2
    sysenter %ios
    syscall
    sysenter %cmanip
    
    mov tlr , 1
    mov fdx , 9
    syscall

    mov stl , %endl
    mov fdx , 2
    sysenter %ios
    syscall

    sysenter %cmanip
    
    mov tlr , 2
    mov fdx , 9
    syscall 

    mov stl , %endl
    mov fdx , 2
    sysenter %ios
    syscall
    sysenter %cmanip
    mov tlr , 3
    mov fdx , 9
    syscall

    mov stl , %endl
    mov fdx , 2
    sysenter %ios
    syscall

```

Output:
```
-1
33
0
0
```

## Threads
Threads are blocks of code declared within the `data` section. Once you switch to the `start` section, they will start executing simultaneously with the source code.
Threads are declared like this:
```asm
. data
    thread  test_thread : {
        ; code
    }
. start
    ; more code

; "code" and "more code" will be executed almost at same time.
```

Since this is a low-level language, there is not much of use of these threads yet.

Example:

`index.asm`:
```asm
. config
    memsize ~ 87
. data
    txt  mytext : "Hello World"

    thread  testthread : {
        __say 0,"thread debug 1"
        __say 0,"thread debug 3"
        __say 0,"thread debug 5"
        __say 0,"thread debug 6"
        __say 0,"thread debug 7"
        __say 0,"thread debug 8"
        __say 0,"thread debug 9"
        __say 0,"thread debug 10"
    }
    thread  testthread2 : {
        __say 0,"thread debug 2"
        __say 0,"thread debug 4" 
    }

. start
    zero stl
    mov tlr , mytext
    mov fdx , 1
    sysenter %ios
syscall

    ;other code
```

Output will be:
```
thread debug 1
thread debug 2
thread debug 3
thread debug 4
thread debug 5
thread debug 6
thread debug 7
thread debug 8
thread debug 9
Hello World             thread debug 10
```

## Instruction set and opcodes
NewASM has a bunch of instructions, and in order to mimic real assemblers, there is a specific opcode linked to every instruction, for example `mov` is `0x6`:

```asm
.start
    0x6 tlr, 1 ; same as `mov tlr, 1`
    ; more code
```