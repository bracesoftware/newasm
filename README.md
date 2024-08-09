# New-Assembly
Below is the simple `Hello World` program written in New-Assembly.

```asm
_ : data
    myvar ; 23
    str ; Hello world
_ : start
    mov . otx , str
    mov . fdx , 1
    
    syscall . 0 , 0

    retn . 0 , myvar
```

# Documentation
Documentation about `newasm` which includes following topics:
- Compiling binaries
- Labels
- Variables
- Instructions

## Compiling
This project is written purely in C++ using its standard libraries, so compiling it should be easy. To download C++ compiler, please follow instructions on the link below:

[Click me...](https://code.visualstudio.com/docs/languages/cpp)

## Labels
Labels are built-in "tags" used to classify code. Each label has different syntax in terms of instructions. General syntax is:

```asm
_ : label_name
```

### `data` label
In this label, you can declare variables to avoid repeated code. General syntax is:

```asm
variable_name ; variable_value
```

You can use `variable_name` as an operand in instructions documented below.

### `start` label
In this label, you can perform instructions, and cannot create variables, or else program will end with exit code 1.

## Instructions
Below is a list of available instructions. General syntax is:

```asm
instruction . suffix , operand
```

This is also valid:

```asm
instruction.suffix,operand
```

### `retn` instruction
Ends your program with specific exit code.

#### Syntax
- `instruction` - `retn`
- `suffix` - no suffix, use `retn . 0 , ...`
- `operand` - exit code

#### Example

```asm
_ : start
    retn . 0 , 23
```

Output:

```
New-Assembly eXecutor 0.0.1 (c) 2024 Brace Software Co., by DEntisT


[newasm] PROGRAM THREAD @ System info: Program finished with exit code : 23
```

### `mov` instruction
Set value of a specific register.

#### Syntax
- `instruction` - `mov`
- `suffix` - register name
- `operand` - new register value

#### Example

```asm
_ : start
    mov . fdx , 1
    retn . 0 , 23
```

#### Register list
| Register name    | Description |
| -------- | ------- |
| `fdx`  | Holds an index of a function `syscall` will call.    |
| `otx` | Used as a text output argument in some `syscall`s.     |

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
    mov . otx , Hello World
    syscall . 0 , 0
    retn . 0 , 23
```

#### `syscall` list
| Register name    | Arguments | Description |
| ---------------- | --------- | ----------- |
| `1` | `otx` | Prints text. |

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
_ : data
    myvar ; 23
    str ; Hello world
    str2 ; Hi again
    str3 ; procedure works

_ : start
    mov . otx , str
    mov . fdx , 1
    
    syscall . 0 , 0

    proc . 0 , testproc
        mov . otx , str3
        mov . fdx , 1
        syscall . 0 , 0
    end . 0 , 0

    mov . otx , str2
    syscall . 0 , 0

    call . 0 , testproc

    retn . 0 , myvar
```

Basically, these are just functions, but in assembly.