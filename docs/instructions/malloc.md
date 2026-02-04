
# Manual memory allocation instructions
These instructions are used to manually allocate memory on the heap and use it.
This article includes the following instructions:
1. [`malloc` and `free`](#malloc-and-free)
2. [`sel`](#sel)
3. [`vmov` and `cast`](#vmov-and-cast)

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