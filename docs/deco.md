- [Go back...](../README.md)

## Decorators
Decorators manipulate how your variables and other features work in the backend.
```asm
.data
	[lock] ; constructive decorator
	; code
	[!lock] ; destructive decorator

```

| Decorator | Description |
| ---------------- | --------- | ----------- |
| `[lock]` | This decorator marks variables locked or hidden from threads. |
| `[volatile]` | This decorator disables the variable caching. |
