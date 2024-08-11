# New-Assembly
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

# Documentation
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
    - [`syscall`](#syscall-instruction)
    - [`nop`](#nop-instruction)
    - [`rem`](#rem-instruction)
    - [`sysreq`](#sysreq-instruction)
    - [`halt`](#halt-instruction)
    - [`push` and `pop`](#push-and-pop-instruction)
    - [`zero`](#zero-instruction)
    - [`heap`](#heap-instruction)
    - [`load`](#load-instruction)
    - [`jmp` and labels](#labels)
- [Procedures](#procedures)
- [Exit codes](#exit-codes)
- [Comments](#comments)

## Compiling
This project is written purely in C++ using its standard libraries, so compiling it should be easy. To download C++ compiler, please follow instructions on the link below:

[Click me...](https://code.visualstudio.com/docs/languages/cpp)

## Arguments
- There are some arguments you can use when executing the interpreter.

| Argument | Parameters | Description |
| ---------------- | --------- | ----------- |
| `-ver` | - | Displays version information only, and doesn't start the interpreter at all. |
| `-help` | - | Displays help information. |
| `-input` | `<filename>` | Sets the input file; if `-input` was not provided, interpreter sets it to `input.nax`. Note that the input file must be located inside the `nax_scripts` folder, file extension doesn't matter, it can be the standard `.asm` as well. |

## Sections
Sections are built-in "tags" used to classify code. Each section uses different syntax in terms of instructions. General syntax is:

```asm
_ : section_name
```

### `config` section
In this section, you can setup some settings for your program. It is thus recommended to keep this section on top of the code. General syntax is:

```asm
_ : config
    configuration @ value_in_specific_datatype
_ : data
    ; some stuff
_ : start
    mov . fdx , 100
    retn . fdx
```

A simple example would be:

```asm
_ : config
    memsize @ 256 ; can be any integer from 1 to 512
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
- `txt` for strings.

### `start` section
In this section, you can perform instructions, and cannot create variables, or else program will end with exit code 1.

## Built-in references
This language brings some built-in references, or rather operands, with itself - list:

- `%ios` - used as an operand in `syscall`, represents a module of `syscall` functions;
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

| Register name    | Description |
| -------- | ------- |
| `fdx`  | Holds an index of a function `syscall` will call. |
| `tlr` | Typeless register (can hold any value, even code literals). Used as an output argument in some `syscall`s. |
| `stl` | Typeless register; however used to hold built-in operands for `syscall`s. |
| `stk` | Stack pointer. |
| `hea` | Heap pointer. |
| `psx` | Procedure scope exit value. |

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

## Comments
Comments are also available:

```asm
_ : start
    ; comment
    mov . fdx , 1
    mov . tlr , "hello" ; comment
    syscall . 0 , %ios ; comment again
```