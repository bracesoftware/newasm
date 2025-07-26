- [Go back...](../README.md)

## Environment variables
Environment variables are typeless variables that are defined within the `.newasm/env_vars.ini` file. Let's take a look:

`.newasm/env_vars.ini`:
```ini
testenv = "hello from env var"
```

`index.asm`:
```asm
.start
    mov tlr , * / testenv
    mov fdx , 1
    sysenter %ios
    syscall
```

* **NOTE**: You must use `*/` before the environment variable name so the program knows you are using an environment variable and not a standard variable, which means this wouldn't work:

```asm
.start
    mov tlr , testenv
    mov fdx , 1
    sysenter %ios
    syscall
```
