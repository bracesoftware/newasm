- [Go back...](../README.md)

## Objects
In this quite frankly low-level language, there are also objects! General syntax is:
```asm
.data
    obj  objure_name : {
        ; members
        data_type  member_name : member_value
    }

```

### Example
```asm
.data
    obj  myobj : {
        num  lol : 98
        decm  decimal : 2.3
        txt  text : "hi from obj"
    }
    obj  myobj2 : {
        num  lol : 45
        decm  decimal : 833.4
        txt  text : "hi from obj again"
        ref  reference : &prptest
    }
.start
    mov tlr , text @ myobj
    mov stl , 0c1
    mov fdx , 1
    sysenter %ios
    syscall

    mov tlr , decimal @ myobj
    mov fdx , 2
    sysenter %ios
    syscall

    mov tlr , lol @ myobj
    mov fdx , 2
    sysenter %ios
    syscall

    mov tlr , text @ myobj2
    mov stl , 0c1
    mov fdx , 1
    sysenter %ios
    syscall

    mov tlr , decimal @ myobj2
    mov fdx , 2
    sysenter %ios
    syscall

    mov tlr , lol @ myobj2
    mov fdx , 2
    sysenter %ios
    syscall

    mov tlr , reference @ myobj2
    mov fdx , 6
    sysenter %ios
    syscall

    mov stl , 45657

    stor stl , lol @ myobj2
    mov tlr , lol @ myobj2
    mov fdx , 2
    mov stl , 0c1
    sysenter %ios
    syscall

    mov psx , "HIII243"

    stor psx , text @ myobj
    mov tlr , text @ myobj
    mov fdx , 1
    mov stl , 0c1
    sysenter %ios
    syscall
```

Output:

```
hi from obj
2.3
98
hi from obj again
833.4
45
prptest
45657
HIII243
```

### Notes
1. Currently, if you want to store a value into a obj member, only the `stor` inobjion will work - note that inobjions such as `load` and `pop` will not work regarding this.
2. This syntax is a must:
```asm
obj  name : { ; brace must be HERE
; and not here
    num  number : 384
}
```
