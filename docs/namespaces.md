- [Go back...](../README.md)

## Namespaces

```asm
.data
	./locals
		[lock]
			txt variable: "hi"
		[!lock]
	./!locals
.start
	mov tlr, locals::variable
	mov stl, 0c1
	mov bos, 2
	mov fdx, 1
	sysenter %ios
	syscall
	ret 0
```

Namespaces give you an intuitive way of moduling code.