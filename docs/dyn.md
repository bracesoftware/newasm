- [Go back...](../README.md)
## Dynamic libraries
**Dynamic libraries** (`.newasm_dl` file) are files that provide user-made instructions. For example, let's say this is a NewASM dynamic library you wrote:

`sayhi.newasm_dl`:
```asm
mov tlr , "hi from my dynamic library"
mov fdx , 1
sysenter %ios
syscall
```

In your entry file - `index.asm`, you can do this:
```asm
.start
    mov tlr , "some stuff"
    inc prp ; bunch of operations
    ; more stuff...

    sayhi ; your very own custom instruction
```

This is literally a fancy way of making procedures, making them reusable across files, however there are limits:
1. You cannot create labels.
2. You thus cannot use jump instructions such as `jmp`, `je`, `jne` and more.

### Runtime errors
If you make an error inside a dynamic library, you will get a runtime error while the library was getting implemented. Below is a list of runtime errors that may occur:

| Runtime error code | Description |
| -------- | ----------- |
| `1` | Tried to create a label. |
| `2` | Tried to use an unsupported instruction. |
