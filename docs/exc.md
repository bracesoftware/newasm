- [Go back...](../README.md)

## Exit codes
When a fatal error happens, program will shut down, returning a specific exit code, below is a list of exit codes.

| Exit code | Description |
| ---------------- | ----------- |
| `0` | No termination point (standard exit code if you have no `ret` or `retn` in your script). |
| `1` | Invalid section (for example, tried to enter a section named `.lol`). |
| `2` | Attempted to call a procedure which does not exist. |
| `3` | Invalid non-numeric value was passed to `retn`. |
| `4` | `sysreq` failed, pretty self-explanatory. |
| `5` | Stack/heap collision; pretty self-explanatory. |
| `6` | Data overflow - tried to pop a value into unallocated address. |
| `7` | Data type mismatch. |
| `8` | Tried to redefine a label. |
| `9` | Bus error; tried to access an invalid label with `jmp`. |
| `10` | Invalid instruction; pretty self-explanatory, also happens if you use `.` or `,` (instruction parsing delimiters) in your `rem` comments. |
| `11` | Memory overflow. |
| `12` | Memory underflow. |
| `13` | Procedure redefinition. |
| `14` | Invalid memory access; tried to access an address using `mov.hea,...` that isn't allocated for the heap. |
| `15` | Invalid syntax. |
| `16` | Memory leak: neither `retn`, `ret` or `heap` were used at the end of the program while the `hea` register wasn't at 0. |
| `17` | Invalid configuration. |
| `18` | You tried to call a procedure within a procedure. |
| `19 `| Unknown system call index (invalid value moved into `fdx`). |
| `20 `| Unknown parser directive. |
| `21 `| Namespace error. |
| `22` | Attempted to use an unassigned pointer/reference variable. |
| `23` | Tried to redefine a variable. |
| `24` | Tried to redefine a structure. |
| `25` | Unexpected closing brace. |
| `26` | Tried to access a structure that hasn't been declared. |
| `27` | Tried to access an undefined structure member. |
| `28` | Tried to create a struct inside a struct. |
| `29` | Expected closing brace. |
| `30` | Tried to create an empty procedure. |
| `31` | Tried to redefine a data structure. |
| `32` | Unexpected usage of the `end` instruction. |
| `33` | Tried to use an improperly loaded dynamic library instruction. |
| `34` | Unknown event specified in the handler section. |
| `35` | Assigned an invalid procedure as an event handler. |
| `36` | Tried to modify a constant. |
| `37` | Operating system-related error. |
| `38` | Memory allocation error. |
| `39` | Tried to modify the memory manually before using `free`. |
| `40` | Invalid stack call. |
| `41` | Handle reassignment. |
| `42` | `sysenter` failed. |
| `43` | Macro redefinition. |
| `44` | Unexpected hash. |
| `45` | Undefined macro. |
| `46` | Unknown system interrupt ID. |
| `47` | Expected `await` - happens when you don't use `await` before using thread system calls, but if the thread finished or returned they will work. |
| `48` | Undefined thread. |
| `49` | Invalid expression. |
| `50` | Virtual memory overflow. |

