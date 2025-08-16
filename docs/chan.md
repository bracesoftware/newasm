- [Go back...](../README.md)

## Thread channels
Thread channels are used for efficient communication between running concurrent threads. In order to use thread channels, manipulate them with `send` and `recv` instructions to set and read data from channels, respectively. To declare a thread channel, do:
```asm
.data
	cont channelName: ?chan
```

Here is an example on how to use them:
```asm
.data
    cont myChannel : ? chan
.start

    thread testChannel -> {
        recv &myChannel
        mov stl, 0c1
        mov fdx, 1
        sysenter %ios
        syscall
    }

    send &myChannel, "Data from the channel"
    await &testChannel
    sysenter %thread
    mov tlr, &testChannel
    mov fdx, 1
    syscall
```

Output:
```
Data from the channel
```