- [Go back...](../README.md)

## Procedures
Procedures allow you to use the same piece of code without having to actually repeat it. General syntax is:

```asm
proc procedure_name
    ; code
end
```

To call the procedure, use:

```asm
call procedure_name
```

- Here is an example:

```asm
.start
    proc test
        halt proc , 1
    end
    call test
    ret 0
```

Basically, these are just functions, but in assembly.