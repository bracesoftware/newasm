# Linker
You can easily link more source files into one using the `link` compile time instruction:

`entry.asm`:
```
.start
    ; doing something
    link "file.asm"
    ; doing something again
```

This will link `file.asm` into the `entry.asm` before execution.

> **NOTE**: You have to provide the linker full path to the file you want to link relative to the `entry.asm`.
