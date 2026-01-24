- [Go back...](../README.md)

## Contexts
Context is a data container, or rather its own type, that can hold different information linked to a specific textual key, which is very similar to hashmaps.

To declare a context, do:

```asm
.data
    ctx myContext: () ; nothing in it
```

You can initialize it while declaring it:

```asm
.data
    ctx myContext: ("key": some_data) ; some_data can be an integer, a float, a string or a character
```

To add new keys, or modify existing ones, use the `merge` instruction:

```asm
.start
    merge &myContext, ("key": 1, "key2": 73.3) ; this instruction will modify `key` and add `key2`
```

If we want to delete `key`, we assign it the `nil` value:

```asm
.start
    merge &myContext, ("key": nil) ; `key` is now unavailable
```

To erase the context completely, and give it new keys, use `mov`:

```asm
.start
    mov &myContext, () ; deletes everything
    mov &myContext, ("lmao": something) ; deletes everything AND adds the new key
```

To access the value of a specific key, do:

```asm
.start
    mov tlr, myContext("key") ; we're indexing it like tuples, but instead of number indexes, we use the key name, just like hashmaps
```