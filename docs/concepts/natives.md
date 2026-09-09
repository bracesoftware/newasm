## Native function calls

You can call supported native functions from the VM by using the `native` instruction descriptor.

```asm
[native] call print ; searches for a native C++ function named print
; function has to be explicitly implemented by the VM to be available
; which means unfortunately you cannot just do [native] call format or smth
```

## Implemented natives

| Descriptor | Info |
| ---------------- | ----------- |
| `print` | Prints whatever is stored in `tlr`. |
