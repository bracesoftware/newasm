- [Go back...](../README.md)

## Containers and data structures
- There are following data structures and containers available:
1. Bit arrays
2. Binary trees

### Bit arrays
- Arrays of bits. Internally, they are just integers, but you are provided `syscall`s to manipulate them as if they were really just arrays of bits, an example:

`index.asm`:
```asm
.data
    cont testbitarr : ?bit_arr
.start
    sysenter "cmanip"
    mov cpt , &testbitarr
    mov tlr , 2
    mov stl , 1
    mov fdx , 4
    syscall 
    mov tlr , 1
    mov fdx , 5
    syscall 
    mov fdx , 2
    mov stl , 0c1
    sysenter "ios"
    syscall
    mov tlr , 2
    mov fdx , 5
    sysenter "cmanip"
    syscall 
    mov fdx , 2
    sysenter "ios"
    syscall
```
Output:
```
0
1
```

* NOTE: To really understand what's going on, check out the list of available system calls.

### Binary trees
- Binary trees are specific arrays containing the parent nodes which have their right and their left child. Example:

`index.asm`:
```asm

.data
    cont testbintree : ?bin_tree
.start
    mov cpt , &testbintree
    sysenter "cmanip"
    mov tlr , 0
    mov stl , 33
    mov fdx , 7
    syscall 
    
    mov tlr , 0
    mov fdx , 9
    syscall 

    mov stl , 0c1
    mov fdx , 2
    sysenter "ios"
    syscall
    sysenter "cmanip"
    
    mov tlr , 1
    mov fdx , 9
    syscall

    mov stl , 0c1
    mov fdx , 2
    sysenter "ios"
    syscall

    sysenter "cmanip"
    
    mov tlr , 2
    mov fdx , 9
    syscall 

    mov stl , 0c1
    mov fdx , 2
    sysenter "ios"
    syscall
    sysenter "cmanip"
    mov tlr , 3
    mov fdx , 9
    syscall

    mov stl , 0c1
    mov fdx , 2
    sysenter "ios"
    syscall

```

Output:
```
-1
33
0
0
```
