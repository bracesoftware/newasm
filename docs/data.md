# Data
There are a few topics covered within this article:
1. [Variables](#variables)
2. [Unions](#unions)
3. [Simple arrays](#simple-arrays)
4. [Objects and classes](#objects-and-classes)

## Variables
There are several types of normal variables in NewASM, they are integers, floats, strings and characters.
Integers and floats take up to 4 bytes of space.
Strings are dynamic, unlike in C, so they take up 4 bytes for the header and the rest bytes are the actual characters.
Characters take 1 byte of memory.

To declare a variable, you do this:
```asm
.data
    intg integer: 0
    float pi: 3.14
    string some_text: "Hello World\n"
    char character: '\t'
```

## Unions
You can also create unions, as in the C language. Check union-related instructions documentation.

## Simple arrays
Also called static objects. There are a few types of simple arrays:
- tuples,
- contexts,
- lists.

### Tuples

Tuples are like vectors in C++, they can grow to any size, but each index can hold an address to a different data type such as a float or a string.

```asm
using "ios"
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
mov tlr, ("ey", 67, 1.2, 'a')
mov &tuple_name, *tlr ; change the whole tuple
```

### Contexts

On the other hand, context is something like a hash-map.

To declare a context, do:

```asm
.data
    ctx myContext: () ; nothing in it
```

You can initialize it while declaring it:

```asm
.data
    ctx myContext: ("key": some_data) ; some_data can be an integer, a float, a string or a character
```

To add new keys, or modify existing ones, use the `merge` instruction:

```asm
.start
    merge &myContext, ("key": 1, "key2": 73.3) ; this instruction will modify `key` and add `key2`
```

If we want to delete `key`, we assign it the `nil` value:

```asm
.start
    merge &myContext, ("key": nil) ; `key` is now unavailable
```

To erase the context completely, and give it new keys, use `mov`:

```asm
.start
    mov &myContext, () ; deletes everything
    mov &myContext, ("lmao": something) ; deletes everything AND adds the new key
```

To access the value of a specific key, do:

```asm
.start
    mov tlr, myContext("key") ; we're indexing it like tuples, but instead of number indexes, we use the key name, just like hashmaps
```

> [!TIP]
> All of these are memory-safe operations!

### Lists

A list is a non-continuous list of primitive objects: integers, floats, characters and strings. It is basically an equivalent of a C++'s standard vector. To declare a list, use the following syntax:

```asm
.data
    list ListName : intg ; or float, char, and string
```

To resize or access a specific index of the list, you have to use `resize` and `lea` instructions, respectively.

```asm
.text
    fetch ListName
    resize this, 10 ; make it 10 elements
    lea this, 3 ; load the third element

    mov this, 3
    mov tlr, *this
```

After using `lea`, the list will return the value stored on index 3. Attempting to modify a list without using `lea` first will cause a crash. After each resize, the index will be also invalidated.

## Containers
These objects/variables are complex data containers that can each be manipulated in their own way.
These are:
- bit arrays (`bit_arr`),
- binary trees (`bin_tree`),
- thread channels (`chan`).

To declare a container, do:

```asm
.data
    cont myContainer: ?chan <-- this creates a channel, 
```

> [!TIP]
> To manipulate containers, we use system calls. Check the kernel documentation for more information. To see docs about channels, check out thread documentation.

## Objects and classes
Raw objects are the most complex data type. But since this is a low-level language, they're rarely used for anything.

To create a raw object, simply do:

```asm
.data
    obj objectIdentifier: {
        intg lol: 98
        float decimal: 2.3
        string text: "hi from struct"
    }
```

To get its value, do:
```asm
mov tlr, objectIdentifier{lol}
```

To change its value, do:
```asm
stor tlr, &objectIdentifier{lol}
```

To create an instanced object, you need a class to use as a blueprint.

```asm
.data
    class student: { ; create an object blueprint
        intg age: 0
        string name: ""
        float gpa: 0.0
    }

    ; create 2 instances of the class
    obj Me: instance &student
    obj Him: instance &student

```