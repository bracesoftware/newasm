
# Data manipulation instructions
These instructions are used to manipulate with data and memory.
This article includes the following instructions:
1. [`mov`](#mov)
2. [`stor`](#stor)
3. [`heap` and `load`](#heap-and-load)
4. [`pop` and `push`](#push-and-pop)
5. [`stack`](#stack)

## `mov`
This instruction allows you to change values of registers and variables.

```asm
.start
	mov tlr, "hello" ; change the tlr register to `"hello"`
	mov &variable, *tlr ; getting register value into variable
```


#### Available register list
- There are input and output registers. Input registers are used to get the input from the user, and output registers are used to store data which will be used as an argument in a `syscall` or an operand in an instruction.

| Register name | Full name | Description |
| ------------- | --------- | ----------- |
| `fdx` | function index | Holds an index of a function `syscall` will call. |
| `tlr` | typeless register | Typeless register (can hold any value, even code literals). Used as an output argument in some `syscall`s. |
| `stl` | secondary typeless register | Typeless register; however used to hold built-in operands for `syscall`s. |
| `bos` | byte output size | Specify the number of bytes you want to print out. Works only on `syscall` 1 in `"ios"`. |
| `stk` | stack pointer | Points at the top of the stack. |
| `hea` | heap pointer | Points at an address in the heap. |
| `psx` | procedure scope exit value | Holds value returned inside a procedure using `halt`. |
| `prp` | procedure pointer | Points at the procedure that was called using `call`. |
| `cpr` | comparsion result register | Holds a value of the comparsion (`cmp`) result; `1` for `equal`, `2` for less and `3` for greater (although there are 6 logical variants of `jmp`, only these 3 cases are required and detected by `cmp`). |
| `cr0` | primary calculation register | Register in which all the calculation results are stored. Read [this](#calculation-instructions) for more information... |
| `cr1` | alternate calculation register | Register which is used as a second operand in calculations. Read [this](#calculation-instructions) for more information... |
| `br0` | primary bit operation register | Register in which all the bitwise calculation results are stored. Read [this](#bit-operations) for more information...  |
| `br1` | alternate bit operation register | Register which is used as a second operand in bitwise calculations. Read [this](#bit-operations) for more information... |
| `cpt` | container pointer | Register holding an address specifically of a container or a data structure. Read [this](#containers-and-data-structures) for more information... |
| `tr0` | primary thread register | Dummy for threads. |
| `tr1` | alternate thread register | Another dummy for threads. |

#### Special character list
Special characters supported by the virtual machine are listed below.
| Character code | Description |
| ----- | ---------------- |
| `0c0` | empty character |
| `0c1` | new line |
| `0c2` | carriage return |
| `0c3` | tabulator |
| `0c4` | alert |
| `0c5` | backspace |

## `stor`
> [!WARNING]
> This instruction will be probably removed in the future.

This is used strictly for objects.

## `heap` and `load`
These instructions are used to store and load stuff on the heap.

The `heap` instruction is used to move up and down the heap memory.
```asm
using "ios"
.data
    intg mynum: 0
.start
    heap 3
    mov &mynum, *hea
    mov tlr, mynum
    mov fdx, 2
    sysenter "ios"
    syscall
```
In examble above, we're reading the value of heap pointer and storing it inside the `mynum` variable.

The `load` instruction allows us to store and load data on the heap. This depends on the value stored in `hea`.

```asm
. data
    float testdecimal: 0.0
. start
    ; If the suffix of the LOAD instruction is `*`,
    ; then we will update the value in the address heap pointer
    ; is pointing to - HOWEVER, if the suffix is `&`, then we will 
    ; store the value in the address heap pointer is pointing to
    ; into some variable in `.data`.

    load *, 736.38 ; hea = something, in this case 736.38
    load &, &testdecimal ; myvar = hea

    mov tlr, testdecimal
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios"
    syscall

    ret 0
```

Output:

```
736.38
```

## `push` and `pop`

These 2 instructions are used to push and pop stuff off the stack memory.


```asm
. data
    intg  myvar2 : 0
. start
    push 273
    ; change myvar2 to something dumb:
    mov &myvar2, *fdx

    pop &myvar2

    mov tlr, myvar2
    mov fdx, 1
    sysenter "ios"
    syscall

    ret 0
```

> [!TIP]
> If you just want to pop the value off the stack, and not store it anywhere, just do:

```asm
    pop nil
```

## `stack`
This instruction is used to clear the call stack information from the stack memory.

```asm
.hndl
    0x827 = procedurename ; assign a hex code to a procedure
.start
    proc procedurename
        mov tlr, <0> ; get the first argument
        mov stl, 0c1
        mov fdx, <1> ; get the second argument
        sysenter "ios"
        syscall
        halt 0
    end
    db stk

    push 1 ; push the sec arg
    push "call stack works" ; push the first arg
    push 0x827 ; call the procedure
    stack ; clear up the stack after the procedure call
    wait 3000
    db stk
    push 1 ; push the sec arg
    push "call stack works again!" ; push the first arg
    push 0x827 ; call the procedure
    stack ; clear up the stack after the procedure call
```