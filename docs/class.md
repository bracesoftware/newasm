
- [Go back...](../README.md)

# Classes

* Added classes and instanced objects:

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