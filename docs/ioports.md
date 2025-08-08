- [Go back...](../README.md)

## I/O ports
I/O ports allow you to communicate with hardware directly for tasks that cannot be done just by using CPU registers. To manipulate with I/O ports, we use `out` and `in` instructions. For example, we can tell the screen to use a color when printing text:

```asm
.start
	mov tlr, 1 ;1 is red
	out 1 ; 1 is IO port for screen text color

	int 0x3 ; autobos
	mov tlr, "Hello World"
	mov stl, 0c1 ; endline
	mov fdx, 1
	sysenter %ios
	syscall
	ret 0
```

### List of available I/O ports

| Port ID | Port name | Description |
| ------------ | ----------- | ----------- |
|1|`txtcol`|Used to send text colour data with `out`. `in` instruction returns `0` if the signal wasn't successful.|
|10|`dskfmat`|Used to format the disk. `out` doesn't require any external parameters. `in` always returns `1`.|
|11|`dskw`|Used to write data on the disk; `out` writes data in the `tlr` register, `stl` is the starting byte. `in` instruction returns `1` if the operation was successful and `0` if the operation failed.|
|12|`dskr`|Used to read data on the disk; `out` reads data from the disk, `tlr` is the starting byte and `stl` is the ending byte. `in` instruction returns the read data into the `tlr`.|