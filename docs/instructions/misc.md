# Miscellaneous instructions
Misc instructions are used for various things.
Instructions covered in this article are listed below:
1. [`rem`](#rem)
2. [`zero`](#zero)
3. [`inc` and `dec`](#inc-and-dec)
4. [`exit`](#exit)
5. [`nop`](#nop)

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

