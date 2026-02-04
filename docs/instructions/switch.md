# Switch values
Supports basic comparsions, integer ranges and types.
Following instructions are covered:
1. [`switch`]
2. [`case`]
3. [`default`]

Example:

```asm
.data
    intg switchInteger : 3234
    string switchText : "hi"
.start

    switch switchInteger
    case 23->mov tlr, "It is 23"
    case 324->mov tlr, "It is 324"
    case 1->mov tlr, "it is 1"
    case 347 .. 9838 -> mov tlr, "it is in range"
    default->mov tlr, "it is default"

    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    switch switchText
    case intg -> mov tlr, "it is a number"
    case string -> mov tlr, "it is text"
    default -> mov tlr, "bleh"

    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall
```
Output:
```
it is in range
it is text
```