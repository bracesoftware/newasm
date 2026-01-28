- [Go back...](../README.md)

## Unassigned pointers or references
```asm
.data
    ref  unassigned_pointer : &%null
```

This reference is left unassigned and an attempt to use it will result in an exception.
