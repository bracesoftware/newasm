## Dynamic memory
Super fun!

```asm

.data
    txt toBeDeleted : "hello"
    ./funny
        num deletedNumber : 89
    ./!funny
.start
    mov tlr, toBeDeleted
    mov stl, 0c1
    sysenter "ios"
    mov fdx, 1
    syscall

    del &toBeDeleted ; delete the mem block it is pointin' to
    del &funny::deletedNumber
    malloc 64
        mov hea, [0]
        load *, "Hello from crazy var!"
        ;load &, &toBeDeleted ; seg fault, we're writing to a memory block we marked as deleted
        movaddr &toBeDeleted, *hea ; correct
        mov tlr, toBeDeleted
        mov stl, 0c1
        sysenter "ios"
        mov fdx, 1
        syscall

        heap 21 ; 16 chars in a string and 4 bytes for a header and 1 byte for a new var
        ; if we had done "heap 20" it would
        ; modify the variable before it,
        ; but we would not get any error
        ; since it is valid code
        load *, 72345
        movaddr &funny::deletedNumber, *hea
        mov tlr, funny::deletedNumber
        mov stl, 0c1
        mov fdx, 2
        syscall

        mov tlr, toBeDeleted
        mov stl, 0c1
        sysenter "ios"
        mov fdx, 1
        syscall
    free
```
Output:
```
Hello from crazy var!
72345
Hello from crazy var!
```

### `movasx` for tuples

You can also change the pointer a member of a tuple is holding, with `lea` and `movasx`:

```asm
lea &tuple, index
del &tuple

lea &tuple, index
movasx &tuple, *hea ; specific addr
```