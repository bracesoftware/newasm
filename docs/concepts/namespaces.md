
## Namespaces
Namespaces give you an intuitive way of moduling code.
```asm
using "ios"
.data
	./locals
		string variable: "hi\n"
	./!locals
.start
	mov tlr, locals::variable
	mov bos, 3
	mov fdx, 1
	sysenter "ios"
	syscall
	ret 0
```
You can, of course, nest them:

```asm
.data
	./nm1
		./nm2
			./nm3
				intg data: 0
			./!nm3
		./!nm2
	./!nm1
.start
	mov tlr, nm1::nm2::nm3::data
```