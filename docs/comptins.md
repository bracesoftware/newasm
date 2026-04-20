# Compile-time directives
There are a few compile-time directives:
1. [`using`](#using)
2. [`def`](#def)
3. [`link`](#link)
4. [`pragma`](#pragma)
5. [`undef`](#undef)
6. [`if ... variants` and `fi`](#compile-time-logic)

## `using`
With this directive, you include kernel modules you're going to use in your project.
```asm
using "ios" ; <-- includes input-output-string module, for printing and inputting text
```

## `def`
You can define constants with this one:
```asm
def myConstant, "hello!\n"
.start
	mov tlr, myConstant
```

## `link`
Using this one, you can link multiple files within one application, which is a core of moduled programming.

```asm
link "path/to/file.asm"
```

> [!WARNING]
> You have to provide a full path to the file, relative to the index file you're providing to the virtual machine.

## `pragma`

> [!NOTE]
> This was added in build 25.

Used to configure compiler behaviour. **Currently has no functional settings!**

## `undef`

> [!NOTE]
> This was added in build 26.

Used to undefine symbols declared using `def`.

```asm
undef symbolName
```

## Compile-time logic

> [!NOTE]
> This was added in build 31.

The NewASM compiler allows following compile-time instructions for implementing very simple logic:
- `ifdef`: checks if a symbol/flag is defined;
- `ifndef`: checks if a symbol/flag is not defined;
- `fi`: used for ending an `if` block. 

If you want to combine if-statements, just nest them, you don't need to use `fi` more than once.

```asm
ifdef SMTH
	ifndef SMTH_ELSE
	; do something
fi
```

There are no `else` variants, you have to end each if-block with `fi`.