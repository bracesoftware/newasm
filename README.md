<div align="center">
    <img height="200" src="readme_resources/newasm_logo.png">
</div>

<h1 align = "center">Welcome to the <i><b>New-ASM</b></i> Wiki</h1>
<h3 align = "center">Interpreted Low-Level Language that Mimics Assembly</h3>

<div align="center" style="border-radius: 5px;">
    <img height="30" align="center" src="readme_resources/brace_product.png">
    <img height="30" align="center" src="readme_resources/cpp_logo.png">
</div>

# Documentation
Below is the simple `Hello World` program written in New-Assembly.

```asm
_ : data
    txt $ string = "Hello world"
_ : start
    mov . tlr , string
    mov . fdx , 1
    
    syscall . 0 , %ios

    retn . 0 , 0
```

# Table of contents
Documentation about `newasm` which includes following topics:
- [Compiling binaries](#compiling)
- [Arguments](#arguments)
- [Sections](#sections)
    - [`config` section](#config-section)
    - [`data` section](#data-section)
    - [`start` section](#start-section)
- [Built-in references](#built-in-references)
- [Instructions](#instructions)
    - [`retn` and `ret`](#retn-and-ret-instructions)
    - [`mov` and `stor`](#mov-and-stor-instructions)
        - [Register list](#available-register-list)
    - [`syscall`](#syscall-instruction)
        - [System call list](#syscall-list)
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
- [Procedures](#procedures)
- [Exit codes](#exit-codes)
- [Comments](#comments)
- [Interesting examples](#interesting-examples)
    - [Writing to a file, and then reading it](#writing-to-a-file-and-printing-its-content)
    - [Creating a child process](#creating-a-child-process)
- [Unassigned references](#unassigned-pointers-or-references)
- [Structures](#structures)
- [Project files](#project-files)

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

## Sections
Sections are built-in "tags" used to classify code. Each section uses different syntax in terms of instructions. General syntax is:

```asm
_ : section_name
```

### `config` section
In this section, you can setup some settings for your program. It is thus recommended to keep this section on top of the code. General syntax is:

```asm
_ : config
    configuration ~ value_in_specific_datatype
_ : data
    ; some stuff
_ : start
    mov . fdx , 100
    retn . fdx
```

A simple example would be:

```asm
_ : config
    memsize ~ 256 ; can be any integer from 1 to 512
```

#### List of available settings
| Setting name | Data type | Description |
| ------------ | --------- | ----------- |
| `memsize` | `num` | Reallocates the number of addresses for the heap and the stack of the program. |

### `data` section
In this section, you can declare variables to avoid repeated code. General syntax is:

```asm
data_type $ variable_name = variable_value
```

You can use `variable_name` as an operand in instructions documented below.

There are 3 data types:
- `num` for integers;
- `decm` for floats;
- `txt` for strings;
- `ref` for references to data symbols (basically a kind of a pointer).

### `start` section
In this section, you can perform instructions, and cannot create variables, or else program will end with exit code 1.

## Built-in references
This language brings some built-in references, or rather operands, with itself - list:

- `%ios` - used as an operand in `syscall`, represents a module of system calls responsible for input and output streaming;
- `%fs` - used as an operand in `syscall`, represents a module of system calls responsible for input and output streaming;
- `%exf` - used as an operand in `syscall`, represents a module of system calls responsible for execution flow (starting child processes);
- `%endl` - line ending, used in `stl`;
- `&%null` - used to leave references/pointers unassigned/uninitialized.
- `%nl` - used as a null operand in some instructions.

**WARNING**: Syntax such as `% ios` is invalid.

## Instructions
Below is a list of available instructions. General syntax is:

```asm
instruction . suffix , operand
```

This is also valid:

```asm
instruction.suffix,operand
```

### `retn` and `ret` instructions
Ends your program with specific exit code.

#### Syntax for `retn`
- `instruction` - `retn`
- `suffix` - no suffix, use `retn . 0 , ...`
- `operand` - exit code

#### Syntax for `ret`
- `instruction` - `ret`
- `suffix` - register name
- `operand` - no operand (`0`)

#### Example for `retn`

```asm
_ : start
    retn . 0 , 23
```

Output:

```
[newasm] PROGRAM THREAD @ System info: Program finished with exit code : 23
```


#### Example for `ret`

```asm
_ : start
    mov . tlr , 8
    ret . tlr
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
_ : start
    mov . fdx , 1
    retn . 0 , 23
```

#### Example `#2`
In this example, we basically do `fdx=1`, `myvar=fdx`, `exit 1`:
```asm
_ : data
    num $ myvar = 0
_ : start
    mov . fdx , 1
    stor . fdx , myvar
    retn . 0 , myvar
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

### `syscall` instruction
Set value of a specific register.

#### Syntax
- `instruction` - `syscall`
- `suffix` - no suffix
- `operand` - no operand

#### Example

```asm
_ : start
    mov . fdx , 1
    mov . tlr , "Hello World"
    syscall . 0 , %ios
    retn . 0 , 23
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
| `%fs` | `1` | `tlr` | Create a directory; with `tlr` being a string containing the directory name. |
| `%fs` | `2` | `tlr` | Remove a directory; with `tlr` being a string containing the directory name. |
| `%fs` | `3` | `tlr` | Create a file; with `tlr` being a string containing the file name. |
| `%fs` | `4` | `tlr` | Remove a file; with `tlr` being a string containing the file name. |
| `%fs` | `5` | `tlr`, `stl` | Overwrite file content; with `tlr` being a string containing the file name, and `stl` being a string containing the new content. |
| `%fs` | `6` | `tlr`, `stl` | Append content to file; with `tlr` being a string containing the file name, and `stl` being a string containing the content to append. |
| `%fs` | `7` | `tlr` | Remove all file content; with `tlr` being a string containing the file name. |
| `%fs` | `8` | `tlr`, `stl` | Read a file line; with `tlr` being a string containing the file name, and `stl` being the line number. Read content is subsequently stored in `tlr`. |
| `%exf` | `1` | `tlr` | Start a new child process; with `tlr` being a string containing the file name. |

### `nop` instruction
Do nothing.

#### Syntax
- `instruction` - `nop`
- `suffix` - no suffix
- `operand` - no operand

#### Example

```asm
_ : start
    nop
    retn . 0 , 23
```

### `rem` instruction
Do nothing. NOT RECOMMENDED TO USE!

#### Syntax
- `instruction` - `rem`
- `suffix` - no suffix
- `operand` - your comment

#### Example

```asm
_ : start
    rem . 0 , "my comment"
    retn . 0 , 23
```

### `sysreq` instruction
Ensure that a symbol is available for further use.

#### Syntax
- `instruction` - `sysreq`
- `suffix` - `proc`, `data`
- `operand` - procedure return value

#### Example

```asm
_ : data
_ : start
    ; Notice how we haven't declared anything in _:data
    sysreq . data , variable
    ; Program will be terminated with exit code 4
    ; Same happens when we try to access a procedure:
    sysreq . proc , some_random_proc
```

### `halt` instruction
Return a value inside a function.

#### Syntax
- `instruction` - `halt`
- `suffix` - `proc`
- `operand` - procedure return value

#### Example

```asm
_ : data
    num $ variable = 0
_ : start
    proc . 0 , testprocedure
        halt . proc , 364
    end
    call . 0 , testprocedure
    stor . psx , variable
    mov . tlr , variable
    mov . fdx , 1
    syscall . 0 , %ios
    retn . 0 , 1
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
_ : data
    num $ myvar2 = 0
_ : start
    push . 0 , 273


    ; change myvar2 to something dumb:
    stor . fdx , myvar2


    pop . 0 , myvar2

    mov . tlr , myvar2
    mov . fdx , 1
    syscall . 0 , %ios

    retn . 0 , 0
```

- **TIP**: If you just want to pop the value off the stack, and not store it anywhere, just do:

```asm
_ : start
    pop . 0 , %nl
```

### `zero` instruction
Resets the register to an invalid value that cannot be used.

#### Syntax
- `instruction` - `zero`
- `suffix` - register name
- `operand` - no operand

#### Example

```asm
_ : start
    zero . stl
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
_ : data
    num $ mynum = 0
_ : start
    heap . 0 , 3
    stor . hea , mynum
    mov . tlr , mynum
    mov . fdx , 2
    syscall . 0 , %ios
```

### `load` instruction
- Load data and store data from the heap and into the heap, respectively.

#### Syntax
- `instruction` - `load`
- `suffix` - `adr`, `ref`
- `operand` - literal value or a reference

#### Example
```asm
_ : data
    decm $ testdecimal = 0.0
_ : start
    ; If the suffix of the LOAD instruction is `adr`,
    ; then we will update the value in the address heap pointer
    ; is pointing to - HOWEVER, if the suffix is `ref`, then we will 
    ; store the value in the address heap pointer is pointing to
    ; into some variable in `_:data`. 
    load . adr , 736.38 ; hea = something
    load . ref , testdecimal ; myvar = hea

    mov . tlr , testdecimal
    mov . stl , %endl
    mov . fdx , 2
    syscall . 0 , %ios

    retn . 0 , 0
```

Output:

```
736.38
```

#### Example `#3`
- If we expand our code, and manually assign addresses before cleaning up the heap, we can do this:

```asm
_ : data
    decm $ testdecimal = 0.0
    decm $ testdecm2 = 0.0
_ : start
    load . adr , 736.38 ; hea = something
    load . ref , testdecimal ; myvar = hea

    mov . tlr , testdecimal
    mov . stl , %endl
    mov . fdx , 2
    syscall . 0 , %ios

    ; Allocate more space:
    heap . 0 , 1
    load . adr , 9821.38 ; hea = 63
    load . ref , testdecm2 ; myvar = hea
    mov . tlr , testdecm2
    mov . stl , %endl
    mov . fdx , 2
    syscall . 0 , %ios
    
    mov . hea , 0 ; manually access the first address
    load . ref , testdecm2 ; myvar = hea
    mov . tlr , testdecm2
    mov . stl , %endl
    mov . fdx , 2
    syscall . 0 , %ios

    mov . hea , 1 ; manually access the second address
    load . ref , testdecm2 ; myvar = hea
    mov . tlr , testdecm2
    mov . stl , %endl
    mov . fdx , 2
    syscall . 0 , %ios

    heap . 0 , -1 ; let all the memory go to avoid getting the memory leak
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
- You can create labels inside `_:start` and then jump to them using the `jmp` instruction. General syntax is:

```asm
_ : start
    _ ! label_name

    ; somewhere
    jmp . 0 , label_name
```

#### Example `#1`

```asm
_ : start
    _ ! labelname
        mov . fdx , 4
        syscall . 0 , %ios
    jmp . 0 , labelname
```

#### Example `#2`
A little too complex example.
```asm
_ : start
    jmp . 0 , label2
    _ ! label
        mov . tlr , "label called"
        mov . fdx , 1
        syscall . 0 , %ios
        mov . fdx , 72
        jmp . 0 , label3
        ret . fdx
    _ ! label2
        mov . tlr , "label2 called"
        mov . fdx , 1
        syscall . 0 , %ios
        jmp . 0 , label
    _ ! label3
        mov . tlr , "label3 called"
        mov . fdx , 1
        syscall . 0 , %ios

    retn . 0 , 3873
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
cmp . fdx , 3 ; check if fdx is 3
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
_ : start
    mov . tlr , 5.8
    cmp . tlr , 1
    je . 0 , equal
    ;jne . 0, notequal
    jl . 0 , less
    jg . 0 , greater
    ;jle . 0 , lesseq
    ;jge . 0 , greatereq

    ; We just want to check if they are either equal, less or greater.

    _ ! equal
        mov . tlr , "EQUAL"
        mov . fdx , 1
        mov . stl , %endl
        syscall . 0 , %ios
        jmp . 0 , endtheprogram

    _ ! notequal
        mov . tlr , "NOT EQUAL"
        mov . fdx , 1
        mov . stl , %endl
        syscall . 0 , %ios
        jmp . 0 , endtheprogram

    _ ! less
        mov . tlr , "LESS"
        mov . fdx , 1
        mov . stl , %endl
        syscall . 0 , %ios
        jmp . 0 , endtheprogram

    _ ! greater
        mov . tlr , "GREATER"
        mov . fdx , 1
        mov . stl , %endl
        syscall . 0 , %ios
        jmp . 0 , endtheprogram

    _ ! lesseq
        mov . tlr , "LESS OR EQUAL"
        mov . fdx , 1
        mov . stl , %endl
        syscall . 0 , %ios
        jmp . 0 , endtheprogram

    _ ! greatereq
        mov . tlr , "GREATER OR EQUAL"
        mov . fdx , 1
        mov . stl , %endl
        syscall . 0 , %ios
        jmp . 0 , endtheprogram

    _ ! endtheprogram
    retn . 0 , 0
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
_ : start
    mov . cr0 , 3.0 ; can be either a float or a whole number
    mov . cr1 , 9 ; same

    add ; cr0 = cr0 + cr1
    sub ; cr0 = cr0 - cr1
    mul ; cr0 = cr0 * cr1
    div ; cr0 = cr0 / cr1
    exp ; cr0 = power(cr0,cr1)
    log ; cr0 = log_c1(cr0)
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
_ : start
    mov . br0 , 1 ; must be a number
    mov . br1 , 1 ; same

    and ; br0 = br0 & br1
    or ; br0 = br0 | br1
    not ; br0 = ~br1
    xor ; br0 = br0 ^ br1
    shl ; br0 = br0 << br1
    shr ; br0 = br0 >> br1
```

### `inc` and `dec` instructions
These instructions are used to increment or decrement register values. Since registers can be of various types, these 2 instructions perform differently.

#### Syntax
```asm
inc.register_name
dec.register_name
```

#### Example `#1`
If you operate on a whole number or rational number register, then it may not be so interesting, it just increments it by 1.

```asm
inc.br0 ; br0++
dec.cr1 ; cr1--
```

If you are operating on a typeless register, these operations will check if the register is holding a whole number, and then apply changes to it. Namely, if you try to increment `tlr` while `tlr` is holding a string, you will get a data type mismatch exception.

However, what if you try to decrement a symbol reference, well - this gets interesting. For instance, if you tried to decrement the `prp` register (which is a reference to a last used procedure), you can't just decrement it by 1 since it is internally pointing to a pair in the procedure map, so the interpreter does the work for you and updates it to the pair before the pair `prp` is pointing at.

Demonstration:
```asm
_ : data
    ref $ temporary = &%null
_ : start
    proc . 0 , procedure_1
        halt . proc , 0
    end
    proc . 0 , procedure_2
        halt . proc , 0
    end
    proc . 0 , procedure_3
        halt . proc , 0
    end

    mov . prp , &procedure_1
    inc . prp

    stor . prp , temporary
    mov . tlr , temporary
    mov . stl , %endl
    mov . fdx , 6
    syscall . 0 , %ios

    mov . prp , &procedure_3
    dec . prp

    stor . prp , temporary
    mov . tlr , temporary
    mov . stl , %endl
    mov . fdx , 6
    syscall . 0 , %ios
```

Output:

```asm
procedure_2
procedure_2
```

## Procedures
Procedures allow you to use the same piece of code without having to actually repeat it. General syntax is:

```asm
proc . 0 , procedure_name
    ; code
end
```

To call the procedure, use:

```asm
call . 0 , procedure_name
```

- Here is an example:

```asm
_ : start
    proc . 0 , test
        halt . proc , 1
    end
    call . 0 , test
    retn . 0 , 0
```

Basically, these are just functions, but in assembly.

## Exit codes
When a fatal error happens, program will shut down, returning a specific exit code, below is a list of exit codes.

| Exit code | Description |
| ---------------- | ----------- |
| `0` | No termination point (standard exit code if you have no `ret` or `retn` in your script). |
| `1` | Invalid section (for example, tried to enter a section named `_:lol`). |
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

## Comments
Comments are also available:

```asm
_ : start
    ; comment
    mov . fdx , 1
    mov . tlr , "hello" ; comment
    syscall . 0 , %ios ; comment again
```

## Interesting examples
Below is a list of interesting examples of using the language.
### Writing to a file, and printing its content

```asm
_ : start
    mov . fdx, 3
    mov . tlr, "filename"
    syscall . 0, %fs
    mov . stl, "TEXTeee"
    mov . fdx, 6
    syscall . 0, %fs
    mov . stl, 1
    mov . fdx, 8
    syscall . 0, %fs
    mov . stl, %endl
    mov . fdx, 1
    syscall . 0, %ios
    retn . 0 , 0
```

Output:

```
TEXTeee
```

### Creating a child process
`index.nax`:
```asm
_ : start
    mov .tlr, "child.nax" ; another nax file containing stuff such as config modifications, variables and procedures
    mov .fdx, 1
    syscall . 0 , %exf ; create a process and execute it

    mov . tlr , "Hi after the process" ; this line will be processed AFTER child.nax finishes executing
    mov . stl , %endl
    mov . fdx , 1
    syscall . 0 , %ios

    retn . 0 , 0
```

`child.nax`:
```asm
_ : data
    num $ mynum = 0
_ : start
    heap . 0 , 36
    stor . hea , mynum
    mov . tlr , mynum
    mov . fdx , 2
    syscall . 0 , %ios ; prints heap size (36)
    heap . 0 , -36 ; free up memory we occupied for the sake of the example
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
_:data
    ref $ unassigned_pointer = &%null
```

This reference is left unassigned and an attempt to use it will result in an exception.

## Structures
In this quite frankly low-level language, there are also structs! General syntax is:
```asm
_:data
    struct $ structure_name = {
        ; members
        data_type $ member_name = member_value
    }

```

### Example
```asm
_ : data
    struct $ mystruct = {
        num $ lol = 98
        decm $ decimal = 2.3
        txt $ text = "hi from struct"
    }
    struct $ mystruct2 = {
        num $ lol = 45
        decm $ decimal = 833.4
        txt $ text = "hi from struct again"
        ref $ reference = &prptest
    }
_ : start
    mov . tlr , text @ mystruct
    mov . stl , %endl
    mov . fdx , 1
    syscall . 0 , %ios

    mov . tlr , decimal @ mystruct
    mov . fdx , 2
    syscall . 0 , %ios

    mov . tlr , lol @ mystruct
    mov . fdx , 2
    syscall . 0 , %ios

    mov . tlr , text @ mystruct2
    mov . stl , %endl
    mov . fdx , 1
    syscall . 0 , %ios

    mov . tlr , decimal @ mystruct2
    mov . fdx , 2
    syscall . 0 , %ios

    mov . tlr , lol @ mystruct2
    mov . fdx , 2
    syscall . 0 , %ios

    mov . tlr , reference @ mystruct2
    mov . fdx , 6
    syscall . 0 , %ios

    mov . stl , 45657

    stor . stl , lol @ mystruct2
    mov . tlr , lol @ mystruct2
    mov . fdx , 2
    mov . stl , %endl
    syscall . 0 , %ios

    mov . psx , "HIII243"

    stor . psx , text @ mystruct
    mov . tlr , text @ mystruct
    mov . fdx , 1
    mov . stl , %endl
    syscall . 0 , %ios
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
struct $ name = { ; brace must be HERE
; and not here
    num $ number = 384
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

Example `index.asm.newasm_proj` file:
```ini
name = Unnamed project
version = 0.0.1
```

