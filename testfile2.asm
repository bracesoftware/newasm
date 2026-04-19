.start
    mov tlr, "Hello from testfile2.asm\n"
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

.data
    string lmao: "0398->hehe,nigga"
    ctx context2: ()
.start
    mov &context2, ("key": lmao)
    mov tlr, context2("key")
    call std::ios::writeln

    mov tlr, lmao
    mov &context2, ("key": *tlr)
    mov tlr, context2("key")
    call std::ios::writeln


    