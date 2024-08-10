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
- [Sections](#sections)
- [Built-in references](#built-in-references)
- [Variables](#data-section)
- [Instructions](#instructions)
    - [`retn` and `ret`](#retn-and-ret-instructions)
    - [`mov` and `stor`](#mov-and-stor-instructions)
    - [`syscall`](#syscall-instruction)
    - [`nop`](#nop-instruction)
    - [`rem`](#rem-instruction)
    - [`sysreq`](#sysreq-instruction)
    - [`halt`](#halt-instruction)
    - [`push` and `pop`](#push-and-pop-instruction)
    - [`jmp` and labels](#labels)
- [Procedures](#procedures)
- [Exit codes](#exit-codes)
- [Comments](#comments)

## Compiling
This project is written purely in C++ using its standard libraries, so compiling it should be easy. To download C++ compiler, please follow instructions on the link below:

[Click me...](https://code.visualstudio.com/docs/languages/cpp)

## Sections
Sections are built-in "tags" used to classify code. Each section uses different syntax in terms of instructions. General syntax is:

```asm
_ : section_name
```

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
    ret . tlr , 0
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
| `fdx`  | Holds an index of a function `syscall` will call.    |
| `tlr` | Used as an integer output argument in some `syscall`s.     |

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
| `%ios` | `1` | `tlr` | Prints exclusively text. |
| `%ios` | `2` | `tlr` | Prints integers and floating point numbers. |
| `%ios` | `3` | - | Requests user input and stores the value in `tlr`. |

### `nop` instruction
Do nothing.

#### Syntax
- `instruction` - `nop`
- `suffix` - no suffix
- `operand` - no operand

#### Example

```asm
_ : start
    nop . 0 , 0
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
    end . 0 , 0
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

### Labels
- You can create labels inside `_:start` and then jump to them using the `jmp` instruction. General syntax is:

```asm
_ ! label_name
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
        ret . fdx , 0
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
end . 0 , 0
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
    end . 0 , 0
    call . 0 , test
    retn . 0 , 0
```

Basically, these are just functions, but in assembly.

## Exit codes
When a fatal error happens, program will shut down, returning a specific exit code, below is a list of exit codes.

| Exit code | Description |
| ---------------- | ----------- |
| `1` | Invalid section. For example, you tried to create a variable inside `_:start`. |
| `2` | Attempted to call a procedure which does not exist. |
| `3` | Invalid non-numeric value was passed to `retn`. |
| `4` | `sysreq` failed, pretty self-explanatory. |
| `5` | Stack underflow - tried to pop a value while the stack was empty. |
| `6` | Data overflow - tried to pop a value into unallocated address. |
| `7` | Data type mismatch. |

## Comments
Comments are also available:

```asm
_ : start
    ; comment
    mov . fdx , 1
    mov . tlr , "hello" ; comment
    syscall . 0 , %ios ; comment again
```