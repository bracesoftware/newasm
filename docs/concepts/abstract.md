## Abstract procedures

> [!WARNING]
> Added in build 34.

Abstract procedures are procedures with no declared body.

```asm
.start
    [abstract] proc myabstractproctestLOL ; creates a proc object
    
    call myabstractproctestLOL  ; throws a recoverable exception, program continues
    
    [impl] proc myabstractproctestLOL ; we get the implementation
        mov tlr, "hello guyz"
        [native] call print
        halt 0
    end
    
    call myabstractproctestLOL ; now it does what it was implemented with
```