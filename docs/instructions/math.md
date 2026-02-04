# Mathematical instructions and bit operations
This package of instructions is used for math (wow!) and manipulating bits (wow again!).
Following instructions are covered in the article:

1. [`add`](#mathematical-instructions-and-bit-operations)
2. [`sub`](#mathematical-instructions-and-bit-operations)
3. [`div`](#mathematical-instructions-and-bit-operations)
4. [`mul`](#mathematical-instructions-and-bit-operations)
5. [`exp`](#mathematical-instructions-and-bit-operations)
6. [`log`](#mathematical-instructions-and-bit-operations)
7. [`and`](#mathematical-instructions-and-bit-operations)
8. [`not`](#mathematical-instructions-and-bit-operations)
9. [`or`](#mathematical-instructions-and-bit-operations)
10. [`xor`](#mathematical-instructions-and-bit-operations)
11. [`shl`](#mathematical-instructions-and-bit-operations)
12. [`shr`](#mathematical-instructions-and-bit-operations)

> [!TIP]
> First 6 of these operations by default use the `cr0` and `cr1` registers. They store the result in `cr0`. They ALWAYS return a `float` value!

Here's a list of what all these do:
```asm
mov cr0, 2
mov cr1, 2

	add ; cr0 : cr0 + cr1
    	sub ; cr0 : cr0 - cr1
    	mul ; cr0 : cr0 * cr1
    	div ; cr0 : cr0 / cr1
    	exp ; cr0 : power(cr0,cr1)
    	log ; cr0 : log_c1(cr0)
```

## Manipulating bits
There are 6 bitwise operations you can use:
- `and` - and,
- `or` - or,
- `not` - not,
- `xor` - exclusive or,
- `shl` - shift left,
- `shr` - shift right.


> [!TIP]
> All of these operations use `br0` and `br1` registers. The result is stored in `br0`; they return `intg`.

```asm
.start
    mov br0 , 1 ; must be a number
    mov br1 , 1 ; same

    and ; br0 = br0 & br1
    or ; br0 = br0 | br1
    not ; br0 = ~br1
    xor ; br0 = br0 ^ br1
    shl ; br0 = br0 << br1
    shr ; br0 = br0 >> br1
```