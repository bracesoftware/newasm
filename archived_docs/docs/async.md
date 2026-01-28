- [Go back...](../README.md)

## `async` procedures

```asm
.start
; async test
    proc TEST
        mov tlr, "hi from async procedure"
        mov stl, 0c1
        mov bos, 2
        mov fdx, 1
        sysenter "ios"
        syscall
        retf 7
    end

    async &TEST ; now TEST proc runs as a thread
    await &TEST

    sysenter "thread"
    mov fdx, 1
    mov tlr, &TEST
    syscall ; display the thread output
```