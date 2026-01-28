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
    mov tlr , myobj{text}
    mov stl , 0c1
    mov fdx , 1
    sysenter "ios"
    syscall

    mov tlr , myobj{decimal}
    mov fdx , 2
    sysenter "ios"
    syscall

    mov tlr , myobj{lol}
    mov fdx , 2
    sysenter "ios"
    syscall

    mov tlr , myobj2{text}
    mov stl , 0c1
    mov fdx , 1
    sysenter "ios"
    syscall

    mov tlr , myobj2{decimal}
    mov fdx , 2
    sysenter "ios"
    syscall

    mov tlr , myobj2{lol}
    mov fdx , 2
    sysenter "ios"
    syscall

    mov tlr , myobj2{reference}
    mov fdx , 6
    sysenter "ios"
    syscall

    mov stl , 45657

    stor stl , &myobj2{lol}
    mov tlr , myobj2{lol}
    mov fdx , 2
    mov stl , 0c1
    sysenter "ios"
    syscall

    mov psx , "HIII243"

    stor psx , &myobj{text}
    mov tlr , myobj{text}
    mov fdx , 1
    mov stl , 0c1
    sysenter "ios"
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


## Tuples (dynamic objects)
Tuples, also known as dynamic objects, can dynamically change sizes and types of its members. Basically an array that can hold any data.
```asm
.$using "ios"
.data
    tuple mytuple: (87, "hi", 64.3, 'p')
    tuple empty_tuple: () ; empty tuple
.start
    mov tlr, mytuble(0) ; access the tuple value at a specific index
    mov tlr, (43, 1.7, "Hi") ; or set `tlr` to be a tuple
```
Tuples can be only one-dimensional.

In order to update the specific index inside a tuple, you have to use the new `lea` instruction:
```asm
lea &tuple_name, 0 ; numeric index
mov tlr, some_value
mov &tuple_name, *tlr ; just update the value at index 0
```
In order to completely change the tuple, just set the specific register to a tuple:
```asm
mov tlr, (“ey”, 67, 1.2, ‘aʼ)
mov &tuple_name, *tlr ; change the whole tuple
```