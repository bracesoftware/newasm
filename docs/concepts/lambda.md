
## Lambda/anonymous procedures
Lambda procedures are procedures that are functioning as arguments in instructions.
```asm
mov tlr, (proc)
	halt 234 ; return a value
(end)
```

Now `tlr` is set to 234.

### Logic in lambdas

> [!WARNING]
> This feature was added in build 31.

Anonymous functions can have their own sealed labels within them, which means you can perform logical operations inside lambdas such as `loop`, `callc`, `jmp` and more.

```asm
.text
    thread thisisfun -> {
        mov tlr, "this is really fun"
        call std::ios::writeln
        mov tlr, 908
        call std::ios::writeln
        mov tlr, 243.4
        call std::ios::writeln

        mov tlr, (proc)
            int 0x3
            mov rax, 4
            mov tlr, "Hi from lambda in thread\n"
            mov fdx, 1
            sysenter "ios"
            syscall
            {:lmao}
            syscall
            loop rax, lmao
            int 0x3
            halt 0
        (end)
    }
```

As you can see, lambdas in threads have their own labels because the JIT compiler ignores the sealed labels within the `(proc)` block.

```asm
.text
	mov tlr, (proc)
		jmp label2
		{:label1}
		jmp label3
		{:label2}
		jmp label1
		{:label3}
		halt 0
	(end)
```