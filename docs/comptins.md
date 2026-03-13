# Compile-time directives
There are a few compile-time directives:
1. [`using`](#using)
2. [`def`](#def)
3. [`link`](#link)
4. [`pragma`](#pragma)
5. [`undef`](#undef)

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