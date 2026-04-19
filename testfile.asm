using "crypto"
.start
    mov tlr, "Hello from testfile.asm\n"
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    mov tlr, "Hello Niggaz"
    sysenter "crypto"
    mov fdx, 1
    syscall
    call std::ios::writeln

    mov tlr, "hi lol"
    mov stl, "key"
    mov fdx, 2 ; encrypt "hi lol"
    sysenter "crypto"
    syscall
    call std::ios::writeln

    mov tlr, "Decryptor says:\n"
    ;mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    sysenter "crypto"

    mov stl, "key"
    mov fdx, 3 ; encrypt "hi lol"
    syscall
    call std::ios::writeln

    link "testfile2.asm"
    