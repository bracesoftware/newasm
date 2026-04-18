
## Macros

You can declare code snippets that get inlined by the compiler.

```asm
MacroName : #
    ; content
    mov tlr, "Hehe"
#

.text
    $MacroName ; compiler will inline the mov tlr, "Hehe" here
```

