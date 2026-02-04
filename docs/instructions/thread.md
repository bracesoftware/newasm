# Thread-related instructions
NewASM virtual machine has built-in concurrency, and there are several ways to use this concurrency.
Instructions covered in this article:

1. [`thread`](#thread)
2. [`await`](#await)
3. [`send` and `recv`](#send-and-recv)
4. [`async`](#async)

## `thread`
Used to spawn a thread.

```asm
.start
	thread threadIdentifier -> {
		mov tlr, "Hello from thread\n"
		int 0x3
		mov fdx, 1
		sysenter "ios"
		syscall
	}
```

## `await`
Block an entire program till a specific thread finishes.

```asm
await &threadIdentifier
```

## `send` and `recv`
Used to manipulate with channels. Channels are data containers used to establish inter-thread communication.

Here's a basic example of using channels:

```asm

.data
    ./threads
        ./lol
            cont myChannel: ? chan ; <-- declare a channel
        ./!lol
    ./!threads
.start

    thread testChannel -> {
        recv &threads::lol::myChannel ; <-- this blocks the thread until it receives data, data is stored in tlr
        mov fdx, 1
        sysenter "ios"
        syscall
    }

    send &threads::lol::myChannel, "Data from the channel\n" ; <-- send data
    await &testChannel ; <-- just wait
    sysenter "thread"
    mov tlr, &testChannel
    mov fdx, 1 ; <-- we see thread output
    syscall
```

## `async`
This is used to create a thread out of a function/procedure.

```asm
async &procedureidentifier
```