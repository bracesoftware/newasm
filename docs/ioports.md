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