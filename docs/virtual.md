
- [Go back...](../README.md)


## Virtual memory
* Added the new virtual memory. Now you can save data and load data from the virtual memory. Virtual memory behaves differently from the heap and the stack. In order to allocate a specific amount of cells in the virtual memory, use `malloc`:
```asm
malloc 27_ ; use the _ operator to tell malloc to REALLOCATE number of cells in the virtual memory
vmov {4}, "Hello" ; in the new vmov instruction we can modify this memory
cast txt ; tell the interpreter to read a string
mov tlr, {4} ; using the {} operator we access the data inside the virtual memory
malloc 0_ ; we can free the memory manually, but not really needed since the program does that for us
```