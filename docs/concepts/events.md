## Events
> [!WARNING]
> This feature was added in build 18.


Events are code blocks that execute automatically when certain things happen in your code.

```asm
.start
	evt 'termination' -> { ; this will automatically append to the event, so different files can use
							: the same identifier
        mov tlr, "Hello guyz! \n This code is lowk insane\t!"
        call std::ios::writeln
    }
```

