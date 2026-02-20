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