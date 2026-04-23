## Address fetching

> [!WARNING]
> This feature was added in build 31.

This article covers the following instructions:
1. [`fetch`](#fetch)

### `fetch`
This instruction is used to fetch a procedure or variable pointer, so we can use it with the `this` keyword. This is very important when optimizing your code because the compiler then can generate code that doesn't do any dictionary lookup.

```asm
.data
    ./data
        intg variable: 4
    ./!data
.text
    fetch data::variable ; get variable's address
    mov this, 6 ; give it a new value
    mov tlr, *this ; dereference the this ptr
    call std::ios::writeln ; prints 6
```

As you can see, we look for the variable only once, and then use the pointer, which means we get around 1.75x faster than using the variable's name twice.

We can also fetch addresses of procedures:

```asm
.text
    fetch std::ios::writeln
    mov tlr, "Hello"
    call this ; prints Hello
    
    mov tlr, 465
    call this
    ; .. etc
    ; this is way faster than letting the dispatcher
    ; look for the procedure every time you use call
    ; callc is still faster tho
```

If you don't want a variable to be fetched, use the `@safe` attribute.
```asm
.data
    @safe
    intg var: 0
.text
    fetch var ; segmentation fault
```


You can also fetch a context or a tuple and use `this` as a handle:

```asm
.text
    fetch tuplename
    mov rax, this(1) ; get second element from the tuple
    lea this, 4 ; load effective address for that tuple when moving at specific index

    fetch ctxname
    merge this, () ; delete all ctxname contents
```

The `this` pointer can be used on `del`, `movasx` and `movaddr` as well.
