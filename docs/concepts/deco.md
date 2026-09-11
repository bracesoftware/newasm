
## Decorators
Decorators manipulate how your variables and other features work in the backend.
```asm
.data
	[private] ; constructive decorator
	; code
	[!private] ; destructive decorator

```

| Decorator | Description |
| ---------------- | ----------- |
| `[private]` | This decorator marks variables locked or hidden from threads. |
| `[volatile]` | This decorator disables the variable caching. |
| `[transient]` | This decorator marks specific data as transient, so they get deleted from memory in the next code section. |

### Descriptors
When you put a decorator in front of an instruction, it becomes an instruction descriptor and can completely alter the instruction's behaviour.

| Descriptor | Info |
| ---------------- | ----------- |
| `[], [nil]` | Empty descriptor. Does absolutely nothing. |
| `[native]` | When used in front of the `call` instruction, you can call native functions implemented by the VM. |

Example:
```asm
[native] call print ; prints anything, way faster than std::ios::writeln or a host service call
```
> [!TIP]
> If you put a certain descriptor in front of an unsupported instruction, it will not cause any issues and will be simple ignored by the virtual machine; which means, descriptors take no hits on the VM's performance.