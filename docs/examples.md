## Interesting examples
This article covers some interesting examples in NewASM programming, for example:
1. [Factorial](#calculating-the-factorial-of-a-number)

### Calculating the factorial of a number

> [!WARNING]
> This works afer build 20.

```asm
.data
    ./std
        ./__
            intg input: 0 ; argument
            intg temp: 0
            intg result: 0
        ./!__
.start
        ./math
            proc factorial
                mov imm, 1
                mov &std::__::input, *tlr ; we take tlr as input lel
                mov &std::__::result, std::__::input ; result = 5
                mov &std::__::temp, std::__::input ; temp = 5
                {:funcBody}
                mov cr2, std::__::temp
                mov cr3, 1
                sub ; temp = temp - 1
                mov &std::__::temp, *cr2 ; temp

                cmp cr2, 0 ; if(temp == 0)
                jz finished ; finished
    
                mov cr3, std::__::temp 
                mov cr2, std::__::result ; cr2 = result
                mul ; result = result * temp
                mov &std::__::result, *cr2 ; save
                
                jmp funcBody

                {:finished}
                mov tlr, std::__::result
                zero imm
                halt 0
            end
        ./!math
    ./!std

    thread TestFactorial -> {
        mov tlr, "factorial of 5 is "
        call std::ios::write
        mov tlr, 5
        call std::math::factorial
        call std::ios::writeln
        mov tlr, "factorial of 10 is "
        call std::ios::write
        mov tlr, 10
        call std::math::factorial
        call std::ios::writeln
        mov tlr, "factorial of 2 is "
        call std::ios::write
        mov tlr, 2
        call std::math::factorial
        call std::ios::writeln
    }

    await &TestFactorial
    mov tlr, &TestFactorial
    sysenter "thread"
    mov fdx, 1
    syscall
```