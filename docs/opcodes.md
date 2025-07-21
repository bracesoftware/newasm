
- [Go back...](../README.md)

## Instruction set and opcodes
NewASM has a bunch of instructions, and in order to mimic real assemblers, there is a specific opcode linked to every instruction, for example `mov` is `0x6`:

```asm
.start
    0x6 tlr, 1 ; same as `mov tlr, 1`
    ; more code
```