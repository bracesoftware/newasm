# Miscellaneous instructions
Misc instructions are used for various things.
Instructions covered in this article are listed below:
1. [`rem`](#rem)
2. [`zero`](#zero)
3. [`inc` and `dec`](#inc-and-dec)
4. [`exit`](#exit)
5. [`nop`](#nop)
6. [`db`](#db)
7. [`cls`](#cls)
8. [`wait`](#wait)
9. [`xchg`](#xchg)
10. [`align`](#align)

## `rem`
Old-styled comments. Not recommended; wasting virtual CPU cycles!
Just use `;`.

```asm
; comment
```

## `zero`
Reset a register.
```asm
zero tlr
```

## `inc` and `dec`
Increase and decrease values of registers.

```asm
inc tlr
```

## `exit`
Terminate the system.

```
exit
```

> [!WARNING]
> This is used stricly in the REPL mode.

## `nop`
Does absolutely nothing.

```asm
nop
```

## `db`
This instruction is used to debug register values.

```asm
db tlr
```

## `cls`
This instruction is used to clear the screen.

```asm
cls
```

## `wait`
This instruction is used to block the main thread for the specific amount of time.

```asm
wait 1000 ; pause for 1s
```

## `xchg`
This instruction exchanges the values of `tlr` and `stl`.

```asm
.start
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

## `align`
Define memory address alignment.

```asm
.start
    align <number>
```