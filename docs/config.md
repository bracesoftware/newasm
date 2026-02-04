# Configuring projects
This article covers following topics:
1. [Project files](#project-files)
2. [NewASM Dynamic Libraries](#dynamic-libraries)

## Project files
Project files (`.newasm_proj` file) are files that define information about your NewASM project. General name format for them is:

```
<entry file name>.newasm_proj
```
For example, if my input file is `index.asm`, project file for that would be `index.asm.newasm_proj`. This is basically an INI file.

> [!TIP]
> Project files are automatically generated when you run the application for the first time.

### Keys and values available
| Key name | Description |
| -------- | ----------- |
| `name` | Name for your project. |
| `version` | Version of your project. |
| `dlibs` | Dynamic libraries your project is using. |

Example `index.asm.newasm_proj` file:
```ini
name = Unnamed project
version = 0.0.1
dlibs = testlib, sayhi
```

> [!TIP]
> If you don't use any dynamic libraries, just do `dlibs = nil`.

## Dynamic libraries
Dynamic libraries (`.newasm_dl` file) are files that provide user-made instructions. For example, let's say this is a NewASM dynamic library you wrote:

`sayhi.newasm_dl`:
```asm
.start
mov tlr, "hi from my dynamic library\n"
mov fdx, 1
sysenter "ios"
syscall
```

In your entry file - `index.asm`, you can do this:
```asm
.start
    mov tlr, "some stuff"
    inc prp ; bunch of operations
    ; more stuff...

    sayhi ; your very own custom instruction
```

This is literally a fancy way of making procedures, making them reusable across files, however there are limits:
1. You cannot create labels.
2. You thus cannot use jump instructions such as `jmp`, `je`, `jne` and more.

### AOT errors
If you make an error inside a dynamic library, you will get an ahead-of-time error while the library was getting implemented. Below is a list of AOT errors that may occur:

| Ahead-of-time error code | Description |
| -------- | ----------- |
| `1` | Tried to create a label. |
| `2` | Tried to use an unsupported instruction. |