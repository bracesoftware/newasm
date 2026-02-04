
## Lambda procedures
Lambda procedures are procedures that are functioning as arguments in instructions.
```asm
mov tlr, (proc)
	halt 234 ; return a value
(end)
```

Now `tlr` is set to 234.