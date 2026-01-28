- [Go back...](../README.md)

# Unions
> Added in build 10.

+ Added the new `union`s! To declare a new union do this:
```asm
.data
    union myUnion: nil ; or anything else
```

Unions are semi-dynamic, which means you can assign any value type to them, but also you have to reallocate (free) memory every time you change their value:

```asm
; to set their value
movas intg ; since it is `nil` 
del &myUnion ; delete the memory

movas string ; new type
mov &myUnion, "Some text" ; now myUnion is a string "Some text", while it was just a number 0
```

## Important notes

+ When we declare an union as `nil` it is actually saved in memory as an integer with value of 0.
+ We use the `movas` instruction to tell the system how we will treat unions.