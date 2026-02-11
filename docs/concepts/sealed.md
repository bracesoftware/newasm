
## Sealed labels

> [!WARNING]
> This feature was added in build 18.

Sealed labels are used to define jump points within thread code blocks, allowing complex multiprocessing.

```asm
.start
    thread testThread -> {
        {:threadLabel} ; sealed label
        mov tlr, "hello world from thread\n"
        mov fdx, 1
        sysenter "ios"
        syscall

        mov tlr, i__
        inc tlr
        mov &i__, *tlr
        cmp tlr, 5
        jnz threadLabel ; label logic in threads so you can have complex multiprocessing
   
        retf 0
    }
```

> [!TIP]
> These also work inside procedures.