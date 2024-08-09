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

### `mov` and `stor` instruction
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
    myvar ; 0
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
| `onm` | Used as an integer output argument in some `syscall`s.     |
| `otx` | Used as a text output argument in some `syscall`s.     |
| `psx` | Holds a value of last procedure scope exit. |

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
| `2` | `onm` | Prints a number. |

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
Do nothing.

#### Syntax
- `instruction` - `rem`
- `suffix` - no suffix
- `operand` - your comment

#### Example

```asm
_ : start
    rem . 0 , my comment
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
    rem . 0 , Notice how we haven't declared anything in _:data
    sysreq . data , variable
    rem . 0 , Program will be terminated with exit code 4
    rem . 0 , Same happens when we try to access a procedure:
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
_ : start
    proc . 0 , testprocedure
        halt . proc , 364
    end . 0 , 0
    stor . psx , variable
    mov . onm , variable
    mov . fdx , 1
    syscall . 0 , 0
    retn . 0 , 1
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