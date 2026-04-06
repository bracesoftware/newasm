## Attributes

> [!WARNING]
> This feature was added in build 23 runtime 8.

Attributes are used to mark certain data with some information, such as if the data is read-only, etc.
You can assign attributes with `@`.

```asm
.data
    @final
    intg myInteger: 0
.start
    mov &myInteger, 23 ; error
```

### List of available attributes
| Identifier | Description |
| ---------- | ----------- |
| `final` | Marks data as read-only. |
| `static` | Marks data immune to move semantics, static variables and objects cannot be moved. Nor can you move data to static variables and objects. |

### Multiple attributes in one line

> [!WARNING]
> This feature was added in build 28.

In order not to waste virtual CPU cycles, instead of doing:

```asm
.data
    @final ; this
    @static ; and this are treated as 2 different instructions,
            ; although they are both applied to 'test'
    intg test: 0
    @static
    intg test2: 0
```

You can just do:
```asm
.data
    @final, static ; now just do all of them once
    intg test: 0
    @static
    intg test2: 0
```