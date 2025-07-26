- [Go back...](../README.md)

# `switch` and `case`
Supports basic comparsions, integer ranges and types.
Example:

```asm
.data
    num switchInteger : 3234
    txt switchText : "hi"
.start

    switch switchInteger
    case 23->mov tlr, "It is 23"
    case 324->mov tlr, "It is 324"
    case 1->mov tlr, "it is 1"
    case 347 .. 9838 -> mov tlr, "it is in range"
    default->mov tlr, "it is default"

    mov stl, ~endl
    mov fdx, 1
    sysenter %ios
    syscall

    switch switchText
    case num -> mov tlr, "it is a number"
    case txt -> mov tlr, "it is text"
    default -> mov tlr, "bleh"

    mov stl, ~endl
    mov fdx, 1
    sysenter %ios
    syscall
```
Output:
```
it is in range
it is text
```