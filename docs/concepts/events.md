## Events
> [!WARNING]
> This feature was added in build 18.


Events are code blocks that execute automatically when certain things happen in your code.

```asm
.start
    evt 'termination' -> { 	; this will automatically append to the event, so different files can use
                            ; the same identifier
        mov tlr, "Hello guyz! \n This code is lowk insane\t!"
        call std::ios::writeln
    }
```

### Built-in events
| Identifier | Description |
| ---------- | ----------- |
| `'termination'` | Called when the program dies. |

Besides using built-in events, you can assign certain procedures their unique hex codes that can be used in the call stack, i.e. when calling functions using the stack.

```asm
.start
    proc myFunction
        ; do smth
        halt 0
    end

    evt 0x9 -> myFunction

    ; now you can do:
    push 0x9
    stack
```