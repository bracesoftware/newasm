- [Go back...](../README.md)


## Threads
Threads are blocks of code declared within the `data` section. Once you switch to the `start` section, they will start executing simultaneously with the source code.
Threads are declared like this:
```asm
.start
    thread test_thread -> {
        ; code
    }
    ; more code

; "code" and "more code" will be executed almost at same time.
```
To avoid interfering with register values inside the `.start` section, threads also, while running simultaneously with `.start`, kind of run for themselves.
Input-output stream system calls inside threads don't output text directly to the console, but save it internally. To display that output we use `%thread` system calls whose require `await` and the new `retf`.

### `await` and `retf` instructions
What `await` does is that it stops the `.start` execution till the thread finishes, while return-far (`retf`) returns a value within the thread.
To print the thread output in the console, you have to use the thread-system calls - same with the returned value.
Exampe:
```asm
.data
    string string : "LOL"

    thread  testthread : {
        mov tlr, "Hello from thread"
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
      
        mov fdx, 1
        sysenter "chrono"
        syscall

        mov fdx, 2
        mov stl, 0c1
        sysenter "ios"
        syscall

        mov tlr, "hi again"
        mov fdx, 1
        syscall

        mov tlr, "this was returned"
        stor tlr, string
        retf string

        sysenter "ios"
        mov tlr, "YOU SHOULD NOT SEE THIS"
        mov stl, 0c1
        mov fdx, 1
        syscall
    }


.start
    await &testthread ; wait for the thread to finish immediatelly
    ;if the thread doesn't finish, we will get ExpectedAwait error
    sysenter %thread
    mov fdx, 1
    mov tlr, &testthread
    syscall ; display the thread output
    mov fdx, 2
    syscall ; fetch returned value
    sysenter "ios"
    mov stl, 0c1
    mov fdx, 1
    syscall ; print the returned val
```

Output:
```
Hello from thread
2025
this was returned
```

