
# Manual memory allocation instructions
These instructions are used to manually allocate memory on the heap and use it.
This article includes the following instructions:
1. [`malloc` and `free`](#malloc-and-free)
2. [`sel`](#sel)
3. [`vmov` and `cast`](#vmov-and-cast)
4. [`movaddr`, `movasx` and `lea`](#movaddr-and-movasx)
5. [`merge`](#merge)

### `malloc` and `free`
Easily manage heap memory. Example:

```asm
.start
    heap 40
    db hea
    malloc 2 ; for test purposes
    mov hea, 2
    db hea
    free nil
    db hea
    __say 0,"2nd malloc"
    malloc 2 ; allocate 2 more spaces in the heap
    mov hea, [1] ; using brackets we can access the address id of the malloc-allocated heap
    db hea
    free nil ; automatically get rid of that allocated memory
    db hea
    heap -40 ; free up the rest we occupied for some testing
```
Output:
```
[NewASM]   PROGRAM THREAD @ Debug | hea = `40`
[NewASM]   PROGRAM THREAD @ Debug | hea = `2`
[NewASM]   PROGRAM THREAD @ Debug | hea = `40`
                        2nd malloc
[NewASM]   PROGRAM THREAD @ Debug | hea = `42`
[NewASM]   PROGRAM THREAD @ Debug | hea = `40`
```

## `sel`
To select what block we want to manipulate, we use `sel`:

```asm
.data
    intg alloc: 0
.start
    malloc 4 ; 4 bytes + 4 byte header
    mov &alloc, *tlr ; tlr has the address of the header

    ; now we use the `alloc` variable to manipulate the allocated memory
    ; `sel` to select it
    ; `free` to free it

    sel alloc
    mov hea, [0]
    ; do smth
    free alloc
```

## `vmov` and `cast`

You can save data and load data from the virtual memory. Virtual memory behaves differently from the heap and the stack. In order to allocate a specific amount of cells in the virtual memory, use `malloc`:

```asm
malloc 27_ ; use the _ operator to tell malloc to REALLOCATE number of cells in the virtual memory
vmov {4}, "Hello" ; in the new vmov instruction we can modify this memory
cast string ; tell the interpreter to read a string
mov tlr, {4} ; using the {} operator we access the data inside the virtual memory
malloc 0_ ; we can free the memory manually, but not really needed since the program does that for us
```

## `movaddr` and `movasx`
Super fun! Using `movaddr` you can manually set the address of a specific variable.

> [!WARNING]
> The `del` instruction is more documented in union-related documentation.

```asm

.data
    string toBeDeleted : "hello"
    ./funny
        intg deletedNumber : 89
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
        mov imm, 1
        load "Hello from crazy var!"
        ; mov imm, 2
        ;load &toBeDeleted ; seg fault, we're writing to a memory block we marked as deleted
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
        mov imm, 1
        load 72345
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

## `merge`
Used for merging contexts.