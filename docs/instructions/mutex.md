## Mutex

> [!WARNING]
> This feature was added in build 33.

This little article covers mutual exclusiveness.
Two instructions that you use to manage *mutex* variables are **lock** and **unlock**.

```asm
.data
    @mutex
    string testinggg: ""
.start
    thread Funnyx -> {
        fetch testinggg
        lock this
        mov &testinggg, "Testing the funny string"
        mov tlr, testinggg
        call std::ios::writeln
        unlock this
    }

    lock &testinggg
    mov &testinggg, "Testing the funny string from main thread"
    mov tlr, testinggg
    call std::ios::writeln
    unlock &testinggg

    await &Funnyx
    mov tlr, &Funnyx
    sysenter "thread"
    mov fdx, 1
    syscall
```

The `mutex` attribute allows you to mark variables as mutually exclusive so that only one thread can manipulate with a variable, forcing other variables to wait for an unlock.