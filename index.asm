def ABSOLUTE_CINEMA, 0
ifdef ABSOLUTE_CINEMA
    link "test/p.asm"
    link "test/g.asm"
fi
.data
    string PrintThisLaterLol: "Test string"
    intg len2 : $ - PrintThisLaterLol
.start
    align 4
    align 3
    int 0x4
    ;cls
:startofprog
using "ios"
using "fs/host"
using "chrono"
using "cmanip"
using "mem"
using "net"
using "thread"
using "txtop"
using "ext"
using "tcp"
using "tuple"
using "http"
using "math"
.start
    try
        throw
    catch nil
    sysenter "cfg"
    mov fdx, 1
    mov rax, 1
    syscall
    mov rax, 1
    syscall
    fetch std::ios::writeln ; set the this ptr
    jmp skiplol
    ./std
        ./lol
            proc testproc
                mov tlr, 1
                mov fdx, 1
                sysenter "ios"
                syscall
                halt 0
            end
        ./!lol
    ./!std

    call std::lol::testproc
    :skiplol

.data
    intg myintgber: 736
    float mydecimal: 243.3
    string mytext: "Hello World"
    string shit: "LOL"
    string return_vals: "null"
    ref testreference: &return_vals ; we must provide a valid value
    string  threadarg : "hello from thread"

.start
    jmp LMAOOOOOOOOOO
    mov tlr, 24.234234.243
    sysenter "math"
    mov fdx, 1
    mov tlr, *tlr
    syscall ; -> error
:LMAOOOOOOOOOO
    thread  testthread -> {
        __say 0,"thread debug 1"
        __say 0,"thread debug 3"
        __say 0,"thread debug 5"
        __say 0,"thread debug 6"
        __say 0,"thread debug 7"
        __say 0,"thread debug 8"
        __say 0,"thread debug 9"
        __say 0,"thread debug 10"
        
        mov tlr, "Hello from thread\n"
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
        ;db stl
        ;db tlr
        ;db fdx
        ;__say 0,"syscall ios"
        mov fdx, 1
        sysenter "chrono"
        syscall
        ;__say 0,"syscall chrono"
        mov fdx, 2
        mov stl, 0c1
        sysenter "ios"
        syscall

        mov tlr, "hi again"
        mov fdx, 1
        syscall
        __say 0, "thread done"
        mov tlr, "this was returned"
        mov &shit, *tlr

        retf shit

        ; this doesn't get executed !
        sysenter "ios"
        mov tlr, "YOU SHOULD NOT SEE THIS"
        mov stl, 0c1
        mov fdx, 1
        syscall
    }
    thread  testthread2 -> {
        __say 0,"thread debug 2"
        __say 0,"thread debug 4"

        mov tlr, "hello from thread 2\n"
        mov bos, 5
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
    }
    thread  testthread3 -> {

        mov tlr, "hello from thread 3\n"
        mov bos, 5
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
    }
    
    int 0x3; auto bos
    await &testthread ; wait for the thread to finish immediatelly
    ;if the thread doesn't finish, we will get ExpectedAwait error
    sysenter "thread"
    mov fdx, 1
    mov tlr, &testthread
    syscall ; display the thread output
    mov fdx, 2
    syscall ; fetch returned value
    __say 0, "dbg 1"
    sysenter "ios"
    __say 0, "dbg 2"
    mov stl, 0c1
    __say 0, "dbg 3"
    mov fdx, 1
    __say 0, "dbg 4"
    db tlr
    syscall
    __say 0, "dbg 5"
    ;db   tr0
    ;db   tr1
    zero stl
    mov tlr , mytext ;test
    ;mov   stl , 0c1
    mov  fdx , 1
    sysenter "ios"
    syscall

    mov  tlr , 0c1
    mov fdx , 5
    syscall

    mov  tlr , mydecimal ; test comment again
    mov  fdx , 2
    syscall 

    mov  tlr , 0c1
    mov  fdx , 5
    syscall 

    mov  tlr , myintgber ; lol
    mov  fdx , 2
    syscall 

    mov  tlr , 0c1
    mov  fdx , 5
    syscall 

    mov tlr, 87
    ;int 0x1 ; realloc
    nop
    nop
    mov tlr, 1
    ;int 0x2

    jmp label3543 ; skip diabolical error check below
    mov tlr, yes
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall
:label3543
    ;mipazuzuzu

.start
    proc  PROCEDURETEST
        __say 0, "PROCEDURETEST called"
        mov  tlr , "Proc works"
        __say 0, "mov tlr, proc works"
        mov  fdx , 1
        __say 0, "mov fdx,1"
        sysenter "ios"
        __say 0, "sysenter 'ios'"
        db tlr
        db fdx
        syscall  
        __say 0, "syscall"  
        mov  tlr , 0c1
        mov  fdx , 5
        syscall 

        halt "this was, in fact, returned"
    end
    nop
    nop
    nop
    call  PROCEDURETEST

    sysreq &return_vals -> string

    mov &return_vals, *psx
    mov  tlr , return_vals
    mov fdx , 1
    syscall  

    mov  tlr , 0c1
    mov  fdx , 5
    syscall

    push  "TEST"
.data
    string  testret : "0"
.start 
    pop &testret
    mov  tlr , testret
    mov  fdx , 1
    syscall 
__say 0,"debug1"
    mov  tlr , 0c1
    mov  fdx , 5
    syscall 
__say 0,"debug2"
    mov  fdx , 873
    mov &myintgber, *fdx
__say 0,"debug3"
    mov  fdx , 3
    syscall 
__say 0,"debug4"
db tlr
    mov fdx , 1
    db tlr
    db fdx
    syscall 
__say 0,"debug5"
    mov  tlr , 0c1
    mov fdx , 5
    syscall  
    __say 0,"debug6"
    mov  fdx , 4
    syscall  

    mov  fdx , 2
    mov  stl , 0c1
    syscall 

    zero  stl 

    push 6
    pop nil

    mov  tlr , "LABEL TEST"
    mov  fdx , 1
    syscall  

    mov  tlr , 0c1
    mov  fdx , 5
    syscall  

    jmp label2

    :label

        mov  tlr , "label called"
        mov  fdx , 1
        syscall  
        mov  tlr , 0c1
        mov  fdx , 5
        syscall  
        mov  fdx , 72
        jmp label3
        ret  fdx 

    :label2

        mov  tlr , "label2 called"
        mov  fdx , 1
        syscall  
        mov  tlr , 0c1
        mov  fdx , 5
        syscall  
        jmp label

    :label3

        mov tlr , "label3 called"
        mov  fdx , 1
        syscall  
        mov  tlr , 0c1
        mov  fdx , 5
        syscall  

    ;pop %nl

    push  863
    heap 4
    heap -3

.data

    intg  register_test : 0

.start

    zero tlr

    mov  tlr , "stk is: "
    mov  fdx , 1
    syscall  
    mov &register_test, *stk
    mov  tlr , register_test
    mov  fdx , 2
    syscall  
    mov  tlr , 0c1
    mov  fdx , 5
    syscall  

    mov  tlr , "hea is: "
    mov  fdx , 1
    syscall  
    mov &register_test, *hea
    mov  tlr , register_test
    mov  fdx , 2
    syscall  
    mov  tlr , 0c1
    mov  fdx , 5
    syscall  

    heap -1

    mov  tlr , "hir"
    mov stl , 0c1
    mov fdx , 1
    syscall  ; hii
    db tlr

    proc  halttest
    __say 0,"halttest called"
        mov  tlr , "proc called"
        mov  stl , 0c1
        mov  fdx , 1
        syscall   ; hi
        halt "halt works"
    end 

    zero  stl 
    zero psx 

    call  halttest

    mov &mytext, *psx
    mov tlr , mytext
    mov stl , 0c1
    mov fdx , 1
    syscall  ; test43

    zero  stl
    mov  tlr, "WORKS"
    mov  stl, 0c1
    mov  fdx , 1
    syscall  

    proc  resetstuff
        mov  stl , "loloool"
        halt 1
    end
.data
    string  teststring : "83"
    float  testdecimal : 0.0
    float  testfloat2 : 3.0
.start

    call  resetstuff
    mov &teststring, *stl
    zero  stl
    mov  stl , 0c1
    mov  tlr , teststring
    mov  fdx , 1
    syscall  

    jmp labeldownthere
    : labeluphere
        mov  stl , 0c1
        mov  tlr , "Hi from up here !"
        mov  fdx , 1
        syscall  
        jmp endofprog
    : labeldownthere
        mov  stl , 0c1
        mov  tlr , "Hi from down here !"
        mov  fdx , 1
        syscall  
        jmp labeluphere
    : endofprog
        mov  stl , 0c1
        mov  tlr , "Bye !"
        mov  fdx , 1
        syscall  
    ;retn   0 , myintgber

    mov  tlr , "heapalloc test"
    mov  fdx , 1
    syscall  

    ; heap   0 , 1 -> no need for this because:
    ; hea : 0
    ; Addresses start from 0, heap pointer is by default at 0
    ;       so to be memory efficient, we don't need to allocate
    ;       more space if we already have 1 cell allocated 
    ; DEPRECATED; NOW WE USE REAL HEAP

    ; If the suffix of the LOAD instruction is `*`,
    ; then we will update the value in the address heap pointer
    ; is pointing to - HOWEVER, if the suffix is `&`, then we will 
    ; store the value in the address heap pointer is pointing to
    ; into some variable in `.data`

    malloc 4 ; allocate 4 bytes
    mov hea, [0] ; set hea to the first byte inside malloc

    ; floats take up 4 bytes
    mov imm, 1
    load 736.38 ; hea = something
    mov imm, 2
    load &testdecimal ; myvar = hea

    free nil ; be responsible

    mov  tlr , testdecimal
    mov  stl , 0c1
    mov  fdx , 2
    syscall 

    ; Allocate more space:
    malloc 4
    mov hea, [0]
    mov imm, 1
    load 9821.38 ; hea : 63
    mov imm, 2
    load  &testfloat2 ; myvar : hea
    
    mov  tlr , testfloat2
    mov stl , 0c1
    mov  fdx , 2
    syscall  

    mov tlr, *hea
    
    ;mov  hea , 0 ; manually access the first address ----------> very bad idea
    mov hea, [0]
    heap 4
    heap -4 ; for testing
    mov imm, 2
    load &testfloat2 ; myvar : hea
    free nil
    
    mov  tlr , testfloat2
    mov  stl , 0c1
    mov  fdx , 2
    syscall  

    ; ANOTHER BAD IDEA mov  hea , 1 ; manually access the second address
    malloc 4
    mov hea, [0]
    mov imm, 2
    load &testfloat2 ; myvar : hea
    mov  tlr , testfloat2
    mov  stl , 0c1
    mov  fdx , 2
    syscall
    free nil

    ;heap -1 ; let all the memory go VERY UNACCURATE

    stor  stl , &testreference
    mov  tlr , testreference
    mov  fdx , 6
    mov  stl , 0c1
    syscall   ; test2342343

    mov  stl , &testreference
    stor  stl , &testreference
    mov  tlr , testreference
    mov  fdx , 6
    mov  stl , 0c1
    syscall   ; test2342343

    proc  reftestproc
        halt &register_test
    end

    call  reftestproc
    stor  psx , &testreference
    mov  tlr , testreference
    mov  stl , 0c1
    mov fdx , 6
    syscall  


    mov  tlr , 5 8
    cmp  tlr , 1
    ;je equal
    ;jne notequal
    jl  less
    jg  greater
    ;jle  lesseq
    ;jge   greatereq

    : equal
        mov  tlr , "EQUAL"
        mov  fdx , 1
        mov  stl , 0c1
        syscall  
        jmp endprogliol

    : notequal
        mov  tlr , "NOT EQUAL"
        mov  fdx , 1
        mov  stl , 0c1
        syscall  
        jmp endprogliol

    : less
        mov  tlr , "LESS"
        mov  fdx , 1
        mov  stl , 0c1
        syscall  
        jmp endprogliol

    : greater
    ;KOMENT
        mov  tlr , "GREATER"
        mov  fdx , 1
        mov  stl , 0c1
        syscall  
        jmp endprogliol

    : lesseq
        mov  tlr , "LESS OR EQUAL"
        mov fdx , 1
        mov  stl , 0c1
        syscall  
        jmp endprogliol

    : greatereq
        mov  tlr , "GREATER OR EQUAL"
        mov  fdx , 1
        mov  stl , 0c1
        syscall  
        jmp endprogliol

    : endprogliol

.data
    intg  cmp_result : 0
.start
    mov &cmp_result, *cpr
    mov tlr , cmp_result
    mov  fdx , 2
    mov  stl , 0c1
    syscall 

    : looool443

    mov  cr0 , 3 0
    cmp  cr0 , 1 3
    jmp xd

    : _lol
        mov  tlr , "lessssss"
        mov  stl , 0c1
        mov  fdx , 1
        syscall  
        jmp endprog3

    : xd
    jl  _lol
    : endprog3
    nop

    mov  cr0 , 2
    mov  cr1 , 3
    exp
    nop

.data
    float exptest: 0.0
.start
    mov tlr, "\n\nexptest:"
    call this
    mov &exptest, *cr0
    mov tlr, exptest
    call this
    mov tlr, "\n\n"
    call this

    mov  br0 , 0
    mov br1 , 87878747

    not

    mov &cmp_result, *br0
    mov  tlr , cmp_result
    mov  stl , 0c1
    mov  fdx , 2
    syscall  

    mov  cr0 , -3 4

    exit

    ;mov br0, "HI"

    proc  repl
        mov tlr, "Called from repl"
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
        mov fdx, "HI"
        call   PROCEDURETEST
        halt 1
    end

 

    mov  tlr , "HI AGAIN"
    mov  stl , 0c1
    mov  fdx , 1
    sysenter "ios"
    syscall  
jmp c3454345343f
    mov prp , &halttest
    mov stl , 0c1
    mov fdx , 6

    dec  prp
:c3454345343f
nop
nop
nop
nop
nop
nop
.data
    ref  prptest : &PROCEDURETEST
    ref  uninref: nil
    ref reference74298374: nil
    float  logtest : 0.0
.start
jmp raspalekidarre
    stor   prp , &prptest
    mov tlr, prptest
    sysenter "ios"
    syscall 
:raspalekidarre
nop
nop
nop
nop
nop
nop

    mov   cr0 , 8.0
    mov   cr1 , 2.0

    log

    mov &logtest, *cr0
    mov   tlr , logtest
    mov   stl , 0c1
    mov   fdx , 2
    sysenter "ios"
    syscall

    ./LMAOOOOOOOOOO
        proc hihi
            halt 0
        end
    ./!LMAOOOOOOOOOO   

.data
    obj  mystruct : { ;comment
        intg  lol : 98
        float  decimal : 2.3
        string  text : "hi from struct"
    }
    obj  mystruct2 : { ;comment
        intg  lol : 45
        float  decimal : 833.4
        string  text : "hi from struct again"
        ref  reference : &LMAOOOOOOOOOO::hihi
    }

.start
    mov   tlr , mystruct{text}
    mov   stl , 0c1
    mov   fdx , 1
    syscall   

    mov   tlr , mystruct{decimal}
    mov   fdx , 2
    syscall   

    mov   tlr , mystruct{lol}
    mov   fdx , 2
    syscall   

    mov   tlr , mystruct2{text}
    mov   stl , 0c1
    mov   fdx , 1
    syscall   

    mov   tlr , mystruct2{decimal}
    mov   fdx , 2
    syscall   

    mov   tlr , mystruct2{lol}
    mov   fdx , 2
    syscall   

    mov   tlr , mystruct2{reference}
    mov   fdx , 6
    syscall   

    mov   stl , 45657

    stor   stl , &mystruct2{lol}
    mov   tlr , mystruct2{lol}
    mov   fdx , 2
    mov   stl , 0c1
    syscall   

    mov   psx , "HIII243"

    stor   psx , &mystruct{text}
    mov   tlr , mystruct{text}
    mov   fdx , 1
    mov   stl , 0c1
    syscall   

    sysenter "fs/host"
    mov   tlr , "unique_ptrtest"
    mov   fdx , 1
    syscall 

    mov   tlr , "unique_ptrtest/file.string"
    mov   stl , "text append"
    mov   fdx , 6
    syscall 

.data
    char  character : 'a'
    obj  test_struct : {
        char  charlol : ','
    } 
.start

    sysenter "ios"
    mov   tlr , character
    mov   stl , 0c1
    mov   fdx , 7
    syscall

    mov   tlr , test_struct{charlol}
    syscall

    mov   tlr , '"'
    syscall   

    mov   fdx , 8
    syscall   

    mov   stl , 0c1
    mov   fdx , 7
    syscall   

    mov   tlr , character
    cmp   tlr , 'a'

    je   chareq
    jl  charle
    jg   chargr

    :chareq
        mov   tlr , "equal"
        mov   stl , 0c1
        mov   fdx , 1
        syscall   
        jmp  endprogcharxd

    :charle
        mov   tlr , "less"
        mov   stl , 0c1
        mov   fdx , 1
        syscall   
        jmp   endprogcharxd

    :chargr
        mov   tlr , "greater"
        mov   stl , 0c1
        mov   fdx , 1
        syscall   
        jmp   endprogcharxd

    :endprogcharxd

    malloc 1
    mov hea, [0]
    mov imm, 1
    load 'p'
    mov imm, 2
    load &character
    free nil

    mov   tlr , character
    mov   stl , 0c1
    mov   fdx , 7 
    syscall   

    proc   charreturn
        halt '>'
    end

    call   charreturn
    mov &character, *psx
    mov   tlr , character
    mov   stl , 0c1
    mov   fdx , 7
    syscall   

    proc  emptyproc
        halt 0
    end
.data
    cont  testbitarr : ?bit_arr
.start
    mov   tlr , "TEST"
    mov   fdx , 1
    syscall   

    mov   tlr , * / testenv
    mov   fdx , 1
    syscall   

    mov   tlr , */testenv2
    syscall   

    testlib
    sayhi

    sysenter "cmanip"

    mov   cpt , &testbitarr
    mov   tlr , 2
    mov   stl , 1
    mov   fdx , 4
    syscall 
    mov   tlr , 1
    mov   fdx , 5
    syscall  
    mov   fdx , 2
    mov   stl , 0c1
    syscall   
    mov   tlr , 2
    mov   fdx , 5
    syscall 
    mov   fdx , 2
    syscall   

    mov   tlr , "Moving on to binary tress   "
    mov   stl , 0c1
    sysenter "ios"
    mov   fdx , 1
    syscall   

.data
    cont testbintree : ? bin_tree

.start
    mov   cpt , &testbintree
 sysenter "cmanip"
    mov   tlr , 0
    mov   stl , 33
    mov   fdx , 7
    syscall   
    ;;;;;;;;;;;
    mov   tlr , 0
    mov   fdx , 9
    syscall 

    mov   stl , 0c1
    mov   fdx , 2
    sysenter "ios"
    syscall   
    ;;;;;;;;;;;;;;;;;;;;;;;;;;
    sysenter "cmanip"
    mov   tlr , 1
    mov   fdx , 9
    syscall  
sysenter "ios"
    mov   stl , 0c1
    mov   fdx , 2
    syscall   
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov   tlr , 2
    sysenter "cmanip"
    mov   fdx , 9
    syscall 
sysenter "ios"
    mov   stl , 0c1
    mov   fdx , 2
    syscall   
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    sysenter "cmanip"
    mov   tlr , 3
    mov   fdx , 9
    syscall 
 sysenter "ios"
    mov   stl , 0c1
    mov   fdx , 2
    syscall   
jmp shjfjsdhfj
    proc  __exit_proc
        mov tlr, "Hiiii"
        call this
        db tlr
        mov   tlr , "exit proc called"
        mov   stl , 0c1
        mov bos, 16
        ;mov bos, 100
        mov   fdx , 1
        sysenter "ios"
        syscall   
        halt 0
    end
:shjfjsdhfj
    evt 'termination' -> {
        mov tlr, "Hiiii"
        call std::ios::writeln
        db tlr
        mov tlr , "exit proc called"
        mov stl , 0c1
        mov bos, 16
        ;mov bos, 100
        mov   fdx , 1
        sysenter "ios"
        syscall
    }

    db   cpt
    db   tlr
    db   hea
    db   stk

    mov   tlr , "hello "
    mov   stl , "world"
    mov   fdx , 1
    sysenter "txtop"
    syscall   
sysenter "ios"
    mov   stl , 0c1
    syscall   
sysenter "txtop"
    mov   tlr ,  "      this is text        "
    mov   fdx , 2
    syscall   
sysenter "ios"
    mov   fdx , 1
    syscall   

    internettest
    netconf

    sysenter "net"
    mov   tlr , "hi com"
    mov   stl , "hi string"
    mov   fdx , 1
    syscall  

    0x6 tlr, "HEX WORKS"
    0x6 fdx, 1
    sysenter "ios"
    mov stl, 0c1
    0x7
    jmp crazy
.data
    intg  nonexistantvar : 1
.start
    mov   tlr , &nonexistantvar
    mov   fdx , 1
    sysenter "mem"
    syscall 
    sysenter "ios"

    mov   tlr , 3
    mov &nonexistantvar, *tlr

    zero   tlr
    mov   tlr , nonexistantvar
    mov   fdx , 2
    syscall   

    ;proc 0,bug
    ;end

    ;mov   tlr , uninref
:crazy
.data
    string testText: ""
.start
    malloc 12
    mov hea, [0]
    mov imm, 1
    load "OhMy\n"
    mov imm, 2
    load &testText
    free nil
    mov tlr, testText
    db tlr
    mov stl, 0c1
    int 0x3
    mov bos, 5
    mov fdx, 1
    sysenter "ios"
    syscall
    int 0x3
    wait 2000

.start
    proc procedurename
        mov tlr, <0> ;get the first argument
        mov stl, 0c1
        mov fdx, <1> ;get the second argument
        sysenter "ios"
        syscall
        halt 0
    end
    evt 0x827 -> procedurename ;assign a hex code to a procedure
    db stk

    push 1 ; push the sec arg
    push "call stack works" ; push the first arg
    mov imm, 1 ; tell the virtual CPU that we're actually calling a function and not pushing the number
    push 0x827 ; call the procedure
    zero imm ; reset the flag
    stack ;clear up the stack after the procedure call
    wait 3000
    db stk
    push 1 ; push the sec arg
    push "call stack works again!" ; push the first arg
    mov imm, 1
    push 0x827 ; call the procedure
    zero imm
    stack ;clear up the stack after the procedure call


.start
    db stk
    proc thisisanerror
        __say 0,"works"
        halt 0
    end
    evt 0x00 -> thisisanerror
    mov imm, 1
    push  0x00
    zero imm
    stack
    db stk

    mov tlr, 23
    mov stl, 56
    xchg
    db tlr
    db stl
.text
    sayhi : #
        mov stl, 0c1
        mov tlr, "hi from macro"
        mov fdx, 1
        sysenter "ios"
        syscall
    #
    debugtlr : #
        db tlr
    #
    sex : #
        __say 0 , "ah"
    #
.start
    $sayhi  ;test
    $ sayhi;test

    $debugtlr ;
    
    $sex
    jmp label2345324

:label2345324
    __say 0,"hi again"
    sysenter "ios"
    mov fdx, 1
    mov tlr, "fuck"
    syscall

.text
    print_dash : #
        mov fdx, 1
        mov tlr, "-"
        zero stl
        sysenter "ios"
        syscall
    #
    print_colon : #
        mov fdx, 1
        mov tlr, ":"
        zero stl
        sysenter "ios"
        syscall
    #
.start
nop
.text
    zero stl
    mov tlr, "Date and time: "
    mov fdx, 1
    sysenter "ios"
    syscall


    mov fdx, 1 ; get year
    sysenter "chrono"
    syscall

    mov fdx, 2 ; print year
    sysenter "ios"
    syscall

    $print_dash

    mov fdx, 2 ; get month
    sysenter "chrono"
    syscall

    mov fdx, 2 ; print month
    sysenter "ios"
    syscall

    $print_dash

    mov fdx, 3 ; get day
    sysenter "chrono"
    syscall

    mov fdx, 2 ; print day
    sysenter "ios"
    syscall

    $print_dash

    mov fdx, 4 ; get hr
    sysenter "chrono"
    syscall

    mov fdx, 2 ; print hr
    sysenter "ios"
    syscall

    $print_colon

    mov fdx, 5 ; get m
    sysenter "chrono"
    syscall

    mov fdx, 2 ; print m
    sysenter "ios"
    syscall

    $print_colon

    mov fdx, 6 ; get s
    sysenter "chrono"
    syscall

    mov fdx, 2 ; print s
    mov stl, 0c1 ; end of string
    sysenter "ios"
    syscall

    mov tlr, (proc)
        __say 0, "ALOOOOOOOOOOOOOO"
        sysenter "ios"
        mov fdx, 1
        mov stl, 0c1
        halt "hello from lambda"
    (end)

    syscall

    mov fdx, (proc)
        halt 1
    (end)
    mov tlr, (proc)
        halt "hi again 2x"
    (end)
    syscall
.data
    intg switchInteger : 3234
    string switchText : "hi"
.start
    mov tlr, "lol->hi"
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    switch switchInteger
    case 23->mov tlr, "It is 23"
    case 324->mov tlr, "It is 324"
    case 1->mov tlr, "it is 1"
    case intg -> mov tlr, "it is a intgber"
    case 347 .. 9838 -> mov tlr, "it is in range"
    default->mov tlr, "it is default"

    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    switch switchText
    case intg -> mov tlr, "it is a intgber"
    case string -> mov tlr, "it is text"
    default -> mov tlr, "bleh"

    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    int 0x3;disable autobos

    mov tlr, " Hello World"
    mov bos, 5 ; now we will just see " Hell"
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    int 0x3 ; activate autobos again

    mov tlr, 1
    mov rax, 1
    out * rax
    mov tlr, "red text"
    mov stl, 0c1
    mov fdx, 1
    syscall

    db tlr
    in 1
    cmp tlr, 0
    db tlr
    db cpr
    jne skip
    __say 0, "fail"
    :skip
    __say 0, "text is red!"

    mov tlr, 8
    out 1

    __say 0, "Dbg 1"

    malloc 243_
    __say 0, "Dbg 2"
    vmov {100}, "Hello World from virtual memory\n"
    __say 0, "Dbg 3"
    cast string
    __say 0, "Dbg 4"
    mov tlr, {100}
    __say 0, "Dbg 5"
    malloc 10_

    sysenter "ios"
    mov stl, 0c1
    mov fdx, 1
    syscall

    sysenter "thread"
    mov fdx, 1
    mov tlr, &testthread2
    syscall ; display the thread output
    sysenter "thread"
    mov fdx, 1
    mov tlr, &testthread3
    syscall ; display the thread output

.data
    string textStringBro : "Allahu Akbar! As-salamu Alaykum Dunya!"
    intg len : $- textStringBro ; calculate
.start
    mov tlr, textStringBro
    mov stl, 0c1
    mov bos, len
    mov fdx, 1
    sysenter "ios"
    syscall

.data
    string looptest : "hello"
    intg msglen : $- looptest
    ./test 
    union testLibUnion: nil
    ./!test
.start
    mov tlr, msglen
    inc tlr
    mov &msglen, *tlr

    __say 0, "msg len is: "
    mov tlr, msglen
    mov stl, 0c1
    mov fdx, 2
    syscall

    int 0x3 ; turn off autobos
    __say 0, "Loop starts:"
    mov bos, 0
    :loop_
    mov tlr, looptest
    mov fdx, 1
    mov stl, 0c1
    sysenter "ios"
    syscall
    inc bos

    cmp bos, msglen
    jl loop_

    int 0x3 ; turn on autobos again
    mov tlr, "finished loop"
    syscall

    ; async test
    proc TEST
        mov tlr, "hi from async procedure"
        mov stl, 0c1
        mov bos, 2
        mov fdx, 1
        sysenter "ios"
        syscall
        retf 7
    end

    async &TEST ; now TEST proc runs as a thread
    await &TEST

    sysenter "thread"
    mov fdx, 1
    mov tlr, &TEST
    syscall ; display the thread output

    mov tlr, "\nDLL moment of truth: "
    call std::ios::writeln
    extern "testlib" ; compiler links our binary with testlib.dll
    mov dlx, "testlib"
    fetch test::testLibUnion ; we need to fetch an union to store our return address
    mov fdx, 1
    sysenter "ext" ; we tell the kernel to look for the function in testlib
    syscall ; we call it

    movas string
    mov tlr, *this
    fetch std::ios::writeln
    call this
    
using "ios"
.data
    [private]
    string myLockedstr : "Locked"
    obj lockedObj : {
        string lockedMember : "LockedMember"
    }
    [!private]
    string unlockedstr : "Unlocked"
.start
    thread mythread -> {
        mov tlr, myLockedstr
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall

        mov tlr, lockedObj{lockedMember}
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall

        mov tlr, unlockedstr
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
    }
.start
    await &mythread
    sysenter "thread"
    mov fdx, 1
    mov tlr, &mythread
    syscall ; display the thread output
jmp kjhdfhdkjs
.data
	./std
    	./disk
            string diskdata : "Hello from disk" ; 15 len
            intg len : $-std::disk::diskdata ; not really needed
    	./!disk
    	./disk2
    	    intg len : 1
        ./!disk2

        obj testObj : {
            string hi : "Hello from std::testObj"
        }
	./!std
.start
    out 10 ; format the disk
    mov tlr, std::disk::diskdata
    mov stl, 100
    out 11 ; write

    mov tlr, 100
    mov stl, 115
    out 12 ; read
    in 12 ; store what we read

    mov fdx, 1
    mov stl, 0c1
    sysenter "ios"
    syscall

	mov tlr, std::disk2::len
	mov stl, 0c1
	mov fdx, 2
	sysenter "ios"
	syscall
	
    mov tlr, std::testObj{hi}
    mov fdx, 1
    syscall
:kjhdfhdkjs
    mov rax, 100
    :lol__
    nop
    loop rax, lol__
.data
    ./tests
        tuple mytuple: ("hi from tuple", 9876, 12.4)
        intg index: 2
        tuple tuple2: (0, 0, 0, 0, 0, 0) ; empty tuple

        obj staticObj: {
           intg nine: 9
           string hello: "Hello from static obj"
        }
    ./!tests

.start
    mov tlr, tests::mytuple(0)
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios" 
    syscall
    ;stor
    mov tlr, tests::mytuple(1)
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios" 
    syscall
    mov tlr, tests::mytuple(tests::index)
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios" 
    syscall

    ; since the tuples are dynamic, we can change their sizes (basically C++ vectors)
    lea &tests::tuple2, 5 ; validify index inside the tuple
    mov tlr, "Hello from modified tuple"
    mov &tests::tuple2, *tlr ; stores the thing

    mov tlr, tests::tuple2(5) ; read the thing we wrote
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    mov tlr, ("hello", 43, 8.9)
    mov &tests::tuple2, *tlr
    mov tlr, tests::tuple2(2) ; 8.9
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios"
    syscall

    mov tlr, tests::staticObj{hello}
    mov fdx, 1
    syscall

    mov tlr, "hello again"
    stor tlr, &tests::staticObj{hello}
    mov tlr, tests::staticObj{hello}
    syscall

    mov stl, "Hello World from stl"
    mov tlr, *stl ; register dereferencing operator
    mov stl, 0c1
    mov fdx, 1 ; js print text
    syscall

    mov tlr, &tests::tuple2
    sysenter "tuple"
    mov fdx, 1
    syscall ; tuple size

    mov fdx, 2
    mov stl, 0c1
    sysenter "ios"
    syscall

    thread myNewThread -> {
        mov tlr, "Hello from myNewThread"
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
        retf 0
    }

    await &myNewThread
    sysenter "thread"
    mov tlr, &myNewThread
    mov fdx, 1
    syscall

.data
    ./threads
        ./lol
            cont myChannel: ? chan
        ./!lol
    ./!threads
.start

    thread testChannel -> {
        recv & threads::lol::myChannel
        ;recv this
        mov fdx, 1
        sysenter "ios"
        syscall

        mov tlr, "\n"
        syscall
    }

    fetch threads::lol::myChannel
    send this, "Data from the channel\n"
    fetch std::ios::writeln
    await &testChannel
    sysenter "thread"
    mov tlr, &testChannel
    mov fdx, 1
    syscall
    jmp label_again
    aaaaaaaa
:label_again
.data
    ./std
        string fuckingshiitttt : "hello+world!"
.start
    mov tlr, std::fuckingshiitttt
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall
.data
        intg LMFAO: $-std::fuckingshiitttt
    ./!std
.start
    
    __say 0, "$- test"
    mov tlr, std::LMFAO
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios"
    syscall

    thread testChannel2 -> {
        recv &threads::lol::myChannel
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
    }
    
    send &threads::lol::myChannel, "Data from the channel, again!"
    await &testChannel2 ; this is an error
    sysenter "thread"
    mov tlr, &testChannel2
    mov fdx, 1
    syscall

.data
    string toBeDeleted : "hello"
    ./funny
        intg deletedintgber : 89
    ./!funny
.start
    mov tlr, toBeDeleted
    mov stl, 0c1
    sysenter "ios"
    mov fdx, 1
    syscall
    
    del &toBeDeleted ; delete the mem block it is pointin' to
    del &funny::deletedintgber
    malloc 64
        mov hea, [0]
        mov imm, 1
        load "Hello from crazy var!"
        ;load &, &toBeDeleted ; seg fault, we're writing to a memory block we marked as deleted
        movaddr &toBeDeleted, *hea ; correct
        mov tlr, toBeDeleted
        mov stl, 0c1
        sysenter "ios"
        mov fdx, 1
        syscall

        heap 25 ; 16 chars in a string and 4 bytes for a header and 1 byte for a new var
        ; if we had done "heap 20" it would
        ; modify the variable before it,
        ; but we would not get any error
        ; since it is valid code
        mov imm, 1
        load 72345
        movaddr &funny::deletedintgber, *hea
        mov tlr, funny::deletedintgber
        mov stl, 0c1
        mov fdx, 2
        syscall

        mov tlr, toBeDeleted
        mov stl, 0c1
        sysenter "ios"
        mov fdx, 1
        syscall
    free nil

    def crazyshit, "Hello again from define"

    mov tlr, crazyshit
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    def tcpaddr, "127.0.0.1:5550" ; compile time evaluation
    mov tlr, tcpaddr ; <- tcpaddr is replaced at compile time
    mov stl, "Hello in tcp"
    mov fdx, 1
    sysenter "tcp"
    syscall ; send tcp req

    mov tlr, tcpaddr
    mov fdx, 2
    syscall ; recv

    mov fdx, 1
    mov stl, 0c1
    sysenter "ios"
    syscall ; print what we got

    mov tlr, #funny::deletedintgber      ; # operator returns the address 
                                        ; object is pointing to
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios"
    syscall

.data
    ./std
        string myTextFromEnv: */helloworld
    ./!std
    
    tuple testTuple: ()
    tuple tobemodified: (73)
.start
    mov tlr, std::myTextFromEnv
    mov stl, 0c1
    mov fdx, 1
    syscall

    mov tlr, "https://google.com"
    mov fdx, 1
    sysenter "http"
    syscall

    sysenter "ios"
    syscall

    mov tlr, "     hi    nigg       "
    mov fdx, 3
    sysenter "txtop"
    syscall

    mov &testTuple, *tlr

    proc printText
        mov fdx, 1
        mov stl, 0c1
        sysenter "ios"
        syscall
        halt 0
    end

    mov tlr, testTuple(0)
    call printText

    mov tlr, testTuple(1)
    call printText
    
    thread testThreadAgain -> {
        mov tlr, "hello from thread hihi"
        call printText
        retf 0
    }

    await &testThreadAgain
    sysenter "thread"
    mov fdx, 1
    mov tlr, &testThreadAgain
    syscall

    jmp hellafun
    :hellafun

    malloc 38
    
    mov hea, [0]
    mov imm, 1
    load 738
    lea &tobemodified, 0
    del &tobemodified ; get rid of memory at index 0
    
    lea &tobemodified, 0
    movasx &tobemodified, *hea ; write new memory MOVAtSpecificindeX

    mov tlr, tobemodified(0)
    mov stl, 0c1
    mov fdx, 2 ; print numbers
    sysenter "ios"
    syscall

    free nil

.data
    [volatile]
    intg number: 3274
    ./std
    ./!std

    class student: { ; create an object that can be instanced
        intg age: 0
        string name: ""
        float gpa: 0.0
    }

    obj Me: instance &student
    obj Him: instance &student
    [!volatile]

    ./namespace
        class testClass: {
            intg lol: 3
        }

        obj testagain:  instance    & namespace::testClass
        obj testagain2:     instance              & namespace::testClass

    ./!namespace
.start
    mov tlr, number
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios"
    syscall

    [volatile] ; just a test

    mov tlr, 4
    stor tlr, & namespace::testagain{lol}

    mov tlr, namespace::testagain{lol}
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios"
    syscall

    mov tlr, namespace::testagain2{lol}
    mov stl, 0c1
    mov fdx, 2
    sysenter "ios"
    syscall

    mov tlr, "EL TARIK"
    stor tlr, &Me{name}

    mov tlr, "RIJAD"
    stor tlr, &Him{name}

    ./std
        ./io
            proc print
                mov stl, 0c0
                mov fdx, 1
                sysenter "ios"
                syscall
                halt 0
            end
        ./!io
    ./!std

    mov tlr, "hello from namespace func"
    call std::io::print

    mov tlr, Me{name}
    call std::io::print
    mov tlr, Him{name}
    call std::io::print

    ./std
        proc testproc
            mov tlr, <0>
            mov stl, 0c1
            mov fdx, 1
            sysenter "ios"
            syscall
            halt 0
        end
    ./!std

    mov tlr, std::version
    mov fdx, 2
    mov stl, 0c1
    sysenter "ios"
    syscall

    proc testproc
        mov tlr, <0>
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
        halt 0
    end
:dhjfjd

.text
    testmacro : #
        switch *tlr
        case intg -> __say 0, "tlr is an integer"
        case float -> __say 0, "tlr is a float"
        case char -> __say 0, "tlr is a char"
        case string -> __say 0, "tlr is a string"
        case ref -> __say 0, "tlr is a lvalue"
        default -> __say 0, "this guy aint nothin"
    #
.start
    ;push 0
    def testreallycool, &gg
    push "Hello from testproc"
    evt 0xff -> std::testproc
    mov imm, 1
    push 0xff
    zero imm
    stack

    mov tlr, (proc)
        halt "I am super cool"
    (end)

    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    mov tlr, 'd'
    $testmacro
:ay645465456
    jmp lol24332243

    ./lol
        proc writexd_
            mov tlr, <0>
            switch *tlr
            case intg -> mov fdx, 2
            case float -> mov fdx, 2
            case string -> mov fdx, 1
            default -> mov fdx, 1

            mov stl, 0c1

            sysenter "ios"
            syscall
            halt 0
        end
    ./!lol
    evt 0xff3 -> lol::writexd_

    push 3
    mov imm, 1
    push 0xff3
    zero imm
    stack

    push "hello"
    mov imm, 1
    push 0xff3
    zero imm
    stack
:lol24332243
    mov tlr, 1
    call this

    mov tlr, "Hello from stdlib"
    call this

    push "Hello"
    pop nil

.data
    ./test
        intg alloc: 0
        intg var: 0
    ./!test
.start
    malloc 12
        mov &test::alloc, *rax ; assign a pointer to a variable, so we can free it later
        mov hea, [0]
        mov imm, 1
        load 73837
        del &test::var
        movaddr &test::var, *hea

        mov tlr, test::var
        call this
    free test::alloc

    ; free 4 ; error
    malloc 1
    free nil

    mov tlr, "omg"
    ; mov bos, 3
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall

    int 0x3
    mov tlr, "Hello from std again and again"
    call this
    int 0x3

    mov tlr, * hea
    call this

	;mov tlr, ui::29042384_w

.data
    union myUnion: nil
    union myUnion2: "Hello"
    intg random: 0
    tuple testTupleOh: (0, 543.534, "oh my God")
.start
    movas intg

    mov tlr, myUnion
    call this

    movas string

    mov tlr, myUnion2
    mov fdx, 1
    mov stl, 0c1
    sysenter "ios"
    syscall

    del &myUnion2 ; note that del also follows the `movas`
    mov &myUnion2, "Npr ja sam super cool lik haha"
    mov tlr, myUnion2
    syscall

    using "misc"
    sysenter "misc"
    mov fdx, 1 ; rand()
    mov tlr, 5 ; min
    mov stl, 10000 ; max
    
    syscall
    mov &random, *tlr
    call this

    mov tlr, #myUnion
    call this

    mov tlr, #student
    call this

    mov tlr, #testTupleOh
    call this

.data
    string testStringBro: "Hello"
    intg myInt: 29083
    string testStringBro2: "Hello"
    intg myInt2: 298374537

    string TetsSTR: "/"

    intg alloc2: 0
.start
    mov tlr, "Address test:"
    call this

    mov tlr, #testStringBro
    call this

    mov tlr, #myInt
    call this

    mov tlr, #testStringBro2
    call this

    mov tlr, #myInt2
    call this

    mov tlr, "Malloc test"
    call this

    malloc 55
    mov &alloc2, *rax
    mov hea, [0]
    mov imm, 1
    load 92873
    sel alloc2 ; select the shi
    call this
    mov hea, [0]
    mov tlr, *hea
    call this
    mov imm, 2
    load &myInt2
    mov tlr, myInt2
    call this
    free alloc2

    movas intg
    del &myUnion
    mov &myUnion, 39 ; memory leak if we don't delete it!

    push "I am a sigma, boy"
    pop &TetsSTR

    mov tlr, TetsSTR
    call this
.data
    ctx myContext: () ; beta feature
    ./test
        ctx myContext2: ()
    ./!test
.start
    mov &myContext, ("key": 1.67, "key2": 843, "lmfao": "ja sam degen")
    mov tlr, myContext("key")
    call this
    mov tlr, myContext("lmfao")
    call this

    mov &myContext, ("hehe":98)

    mov tlr, myContext("hehe")
    call this

    mov &test::myContext2, ("yay":"yay called!")
    mov tlr, test::myContext2("yay")
    call this

    mov &test::myContext2, ()

    merge &test::myContext2, ("yay": 23)

    mov tlr, test::myContext2("yay")
    call this

    merge &myContext, ("hehe" : nil, "kjdhf": "VEOMA KORISNA STVAR JBG") ; hehe will be deleted cuz we set it to nil
    mov tlr, myContext("kjdhf")
    call this

    merge &myContext, ("hehe": 387) ; fix invalid memacc -> we give it a new value
    mov tlr, myContext("hehe") ; <- invalid memory access error 
    call this

    merge &test::myContext2, ("test" : 243)
    mov tlr, test::myContext2("test")
    call this

    merge &test::myContext2, ("test": "very cool")
    mov tlr, test::myContext2("test")
    call this

    merge &test::myContext2, ("test": nil, "test": "very cool again") ; this should work again!
    mov tlr, test::myContext2("test")
    call this
.data
    [transient]
        intg mynum: 43
    [!transient]
.start
    mov tlr, mynum
    call this
.data
    intg mynum2: 29843
.start
    mov tlr, mynum ; will display 29843
    call this

    ;idk
    mov tlr, "Hey"
    mov stl, 0c0

link "testfile.asm" ; link a file

    using "ctx"
    mov tlr, &test::myContext2
    mov fdx, 1
    sysenter "ctx"
    syscall

    call this

    link "testfile3.asm"
    jmp hehe
    :lel__________
    mov tlr, "labele rade fino Allaha mi\n"
    call this
    ;LMFAO TEST
:hehe

    mov tlr, 38356
    mov stl, -25
    call std::math::iadd
    call this

    mov tlr, 38356
    mov stl, -25
    call std::math::isub
    call this
.data
    ./hi
        intg age: 19
        tuple humanData: ("ele bre", "nigga")
        ctx myContext3: ("some_data": "l\tel")
    ./!hi
.start
    mov tlr, "\nHello, I am {hi::humanData(0)} {hi::humanData(1)},\n I am {hi::age} yrs old\n{hi::myContext3("some_data")}"
    sysenter "txtop"
    mov fdx, 4
    syscall

    call this

    mov tlr, "\tHello my people!"
    call std::ios::write
    mov tlr, "I love newasm\a"
    call this
    mov tlr, "I am cool"
    call this
    mov tlr, "I am cool 2"
    call this

.data
    string testToPrint: "Hello World\n"
    intg length: $ - testToPrint
    intg i__: 0
    ./namespace
        char lmao: 'p'
    ./!namespace
.start
    int 0x3
    mov tlr, testToPrint
    mov bos, length
    mov fdx, 1
    sysenter "ios"
    syscall
    syscall
    
    mov tlr, 3
    out 1

    mov tlr, "hello black world\n"
    call this
    
    evt 'termination' -> { ; this will automatically append to the event, so different files can use it
        mov tlr, "\nHello guyz! \n This code is lowk insane\t!"
        call this
    }
;jmp skipallthishsit
    ./lol
        proc writexd
            halt 0
        end
    ./!lol
    ./tredz
    thread testThread -> {
        {:threadLabel} ; new sealed labels coming
        mov tlr, "hello world from thread\n"
        mov fdx, 1
        sysenter "ios"
        syscall
        ;call this
        mov tlr, i__
        inc tlr
        mov &i__, *tlr
        cmp tlr, 5
        jnz threadLabel ; label logic in threads so you can have complex multiprocessing
        ;p
        retf 0
    }
    ./!tredz
;jmp dsfdsfdsdssdf
    mov tlr, "HERE\? IT CRASHES!!! 1"
    call this
    sysreq & tredz::testThread -> thread
    mov tlr, "HERE IT CRASHES!!! 2"
    call this
    sysreq &lol::writexd -> proc
    mov tlr, "HERE IT CRASHES!!! 3"
    call this
    sysreq &std::ios::writeln -> proc
    mov tlr, "HERE IT CRASHES!!! 4"
    call this
    sysreq &namespace::lmao -> char
    mov tlr, "HERE IT CRASHES!!! 5"
    call this
jmp doneshit
jmp lmao
.data
    intg k: 0
.start
:doshit
    mov tlr, k
    cmp tlr, 100
    jz doneshit
    mov fdx, 2
    syscall
    mov tlr, k
    inc tlr
    mov &k, *tlr
    jmp doshit

:doneshit

    ;sysreq &wee
:dsfdsfdsdssdf
    await &tredz::testThread
    mov tlr, &tredz::testThread
    sysenter "thread"
    mov fdx, 1
    syscall
:skipallthishsit
.start
    mov tlr, "::ifac->factorial of 5 is "
    call std::ios::write
    mov tlr, 5
    call std::math::ifac ; <- REAL STANDARD LIBRARY FUNC!
    call this
    mov tlr, "::ifac->factorial of 0 is "
    call std::ios::write
    mov tlr, 0
    call std::math::ifac ; <- REAL STANDARD LIBRARY FUNC!
    call this
    mov tlr, "::ifac->factorial of 1 is "
    call std::ios::write
    mov tlr, 1
    call std::math::ifac ; <- REAL STANDARD LIBRARY FUNC!
    call this
.data
    ./std
        ./__
            intg input: 0 ; argument
            intg temp: 0
            intg result: 0
        ./!__
.start
        ./math
            proc factorial ; <- std::ifac PROTOTYPE!!
                mov imm, 1
                mov &std::__::input, *tlr ; we take tlr as input lel
                mov &std::__::result, std::__::input ; result = 5
                mov &   std::__::temp, std::__::input ; temp = 5
                mov &std::__::temp, std::__::input ; testing compiler's optimization capabilities
                mov rbx, 3.0
                mov rbx, 3.0
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
        fetch std::ios::writeln
        jmp main
        {:func}
            mov tlr, "Hello from TestFactorial::func"
            call this
        retc
        {:main}
        mov tlr, "factorial of 5 is "
        call std::ios::write
        mov tlr, 5
        call std::math::factorial
        call this
        mov tlr, "factorial of 10 is "
        call std::ios::write
        mov tlr, 10
        call std::math::factorial
        call this
        mov tlr, "factorial of 2 is "
        call std::ios::write
        mov tlr, 2
        call std::math::factorial
        call this
        callc func
        nop
    }

    await &TestFactorial
    mov tlr, &TestFactorial
    sysenter "thread"
    mov fdx, 1
    syscall

    thread thisisfun -> {
        mov tlr, "Thisisfun begins here"
        call std::ios::writeln
        try
            fetch nil
            mov this, 0
            nop
        catch errr
        {:lmaox}
        fetch std::ios::writeln
        resb 4
        push 535
        mov tlr, "now we're moving on to call stack"
        call this
        resb nil
        resb 4873
        push 1 ; push the sec arg
        push "call stack works" ; push the first arg
        mov imm, 1 ; tell the virtual CPU that we're actually calling a function and not pushing the number
        push 0x827 ; call the procedure
        zero imm ; reset the flag
        stack ;clear up the stack after the procedure call
        ;wait 3000
        db stk
        push 1 ; push the sec arg
        push "call stack works again!" ; push the first arg
        mov imm, 1
        push 0x827 ; call the procedure
        zero imm
        stack ;clear up the stack after the procedure call
        mov rax, 2
        {:shee}
        mov tlr, "this is really fun"
        call this
        mov tlr, 908
        call this
        mov tlr, 243.4
        call this

        jmp label2
		{:label1}
		jmp label3
		{:label2}
		jmp label1
		{:label3}

        mov tlr, (proc)
            int 0x3
            mov imm, 4
            mov tlr, "Hi from lambda in thread\n"
            mov fdx, 1
            sysenter "ios"
            syscall
            {:lmao}
            syscall
            loop imm, lmao
            int 0x3

            jmp label2
            {:label1}
            jmp label3
            {:label2}
            jmp label1
            {:label3}
            
            halt 0
        (end)
        fetch std::ios::writeln
        db rax
        loop rax, shee

        retf 0

        {:errr}
        mov tlr, "err code is "
        call std::ios::write
        mov tlr, *rax
        call std::ios::writeln
        jmp lmaox
    }
    await &thisisfun
:p
    mov tlr, (proc)
        int 0x3
        
        mov imm, 5
        mov tlr, "Hi from lambda in main thread\n"
        mov fdx, 1
        sysenter "ios"
        
        {:lmao}
        syscall
        loop imm, lmao

        try
            fetch nil
            mov this, 0
            nop
        catch errr
        {:lmaox}
        int 0x3
        halt 0
        {:errr}
        mov tlr, "err code is "
        sysenter "ios"
        mov fdx, 1
        syscall
        mov tlr, *rax
        mov fdx, 2
        syscall
        mov tlr, "\n"
        mov fdx, 1
        syscall
        jmp lmaox
    (end)

    fetch std::ios::writeln

    mov tlr, 763
    call this
    mov tlr, "hellouzz"
    call this
    mov tlr, 'o'
    call this

    await &thisisfun
    mov tlr, &thisisfun
    sysenter "thread"
    mov fdx, 1
    syscall
    try
    catch yay
    jmp yay
    g
    :yay
    mov tlr, #PrintThisLaterLol ; takes a pointer to the string
    mov bos, 56456
    mov fdx, 1
    sysenter "ios"
    syscall

    mov tlr, "HELLOOOOO\n"
    call std::ios::write
.data
    @ final
    intg ThisIsConst: 456
.start
    jmp d787234
    mov &ThisIsConst, 3 ; error

    :d787234
    ;int 0x5
    mov bos, 0b11110100001001000000;1000000
    :loop___
    cmp bos, 0
    jz end____
    dec bos
    ;nop
    nop
    mov rax, 2
    jmp loop___
    :end____
    sysenter "mem"
    syscall
    mov tlr, *rax
    call this
.data
    string testStringMan: "This is absolutely crazy!"
pragma errtest
.start
    sysenter "mem"
    syscall
    mov tlr, *rax
    call this

    mov tlr, "Hello world\n"
    mov rax, 4
    :test4
    call std::ios::write
    loop rax, test4

    mov tlr, "Hello"
    mov rax, 2
    mov fdx, 7
    sysenter "txtop"
    syscall
    call this ; should print 'l'

    mov tlr, '\n'
    sysenter "misc"
    mov fdx, 2
    syscall
    mov tlr, *rax
    call this

    def defineTest, "Hi\tbro\n"
    mov tlr, defineTest
    call this
    undef defineTest
    ;mov tlr, defineTest ; runtime error
    call this
    ;undef pdns ; compile time error
jmp e2349083l
    ;mov tlr, $-&testStringMan
    ;call this
    using "fs/vdsk"
    mov fdx, 1
    mov tlr, "file1"
    sysenter "fs/vdsk"
    syscall
    mov rax, *rax ; new feature, first type of optimization, this line is not included in the binary
    :e2349083l
.data
    @static, final
    intg test: 0
    ctx old: ("key": "Absolute cinema", "mjau": "meow")
    ./lol
        ctx new: ()
        intg test2: -23
    ./!lol
.start
    ;mov tlr, pddsfdssd
    ;mov tlr, jdhsf(3)
    ;mov tlr, sdfoj("ksj")

    mov tlr, old("key")
    call this
    movx &lol::new, &old
    mov tlr, lol::new("key")
    call this
    ;mov tlr, old("key")
    call this

    mov tlr, lol::new("mjau")
    call this

    rem hi ; this is idiotic instruction, its gonna get removed by the compiler

    callc std::345345::FkinFunction
    mov tlr, "Hello from main"

    call this
    int 0x3

    proc AbSoluteCinema
        jmp main
        {:func}
        mov tlr, "\tHello from absolutecinema func\n"
        sysenter "ios"
        mov fdx, 1
        syscall
        retc
        ;{:main}
        {:main}
        callc func
        halt 0
    end
    testmacroagain : #
        $print_colon
        ;:lmaoxd ; this is an compile time error, you can't cover labels with macros
        mov tlr, "HEHE, macro inlining works!\n"
        ;call this
        mov fdx, 1
        sysenter "ios"
        syscall
        $print_colon
        ;call myfunc
    #
    $sayhi
    $testmacroagain

    proc AbsoluteMacrotest
        $testmacroagain
    end
    call AbsoluteMacrotest

    call AbSoluteCinema
    mov rax, 5
    callc Loop

    ifdef ABSOLUTE_CINEMA
    mov tlr, "ABSOLUTE_CINAME IS DEFINED"
    call this
    fi ; the compiler doesn't really check for these, 
        ; fi just resets the internal compiler flag
    ifdef dsjf
    dsdsjf
    fi
    jmp 78927347
    nop
    :78927347
    fetch nil
    mov this, *this
    fetch lol::test2 ; lookup once
    mov this, 424312 ; use the same memory address twice
    mov tlr, *this
    call std::ios::writeln
    mov tlr, "1"
    mov tlr, 1
.data
    @safe
    float lol: 4.6
    intg ramdagadam : 47
    ./interesting
        tuple lmao: ("hello", 32, 5.3, 23, "this is insane", "lmao")
    ./!interesting
    @ mutex
    string testinggg : "Lmao"
.text
    ;fetch lol ; crash, lol is labelled as @safe
    thread interesting -> {
        fetch ramdagadam
        mov rax, 100
        {:f}
        nop
        loop rax, f
    }
    mov rax, 5
    :kkkkkk
    nop
    loop rax, kkkkkk
    await &interesting
    fetch ramdagadam

    fetch interesting::lmao
    mov tlr, this(4)
    call std::ios::writeln
    lea this, 0
    mov this, "hi from modified lea this"
    mov tlr, this(0)
    call std::ios::writeln
    fetch lol::new
    mov tlr, this("mjau")
    call std::ios::writeln
    merge this, ("mjau": "mjau mjau from merge this!")
    mov tlr, this (  "mjau"  )
    call std::ios::writeln
    mov tlr, # this
    call std::ios::writeln
    ;movaddr this, -1

    using "cfg"
    mov tlr, *rax
    push ramdagadam
    mov &ramdagadam, 23
    pop &ramdagadam
    mov tlr, ramdagadam
    
    call std::ios::writeln
jmp 927349872l
    thread Lmao759837 -> {
        mov tlr, "Hello bro"
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
    }

    thread _Lmao759837 -> {
        mov tlr, "Hello broopooooooooooo"
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
        call std::ios::writeln
    }
:927349872l
    proc ProcWithError
        try
            pddsfdssd ; purposefully try to use a non-existing dyn lib
        catch nil ; just continue
        mov tlr, "hi from ProcWithError"
        sysenter "ios"
        sysenter "ios"
        mov fdx, 1
        syscall
        halt 0
    end

./eh
    try
        call ProcWithError
    catch kids
./!eh

; -------------------------- END OF PROGRAM -------------------------- ;
mymacro : #
    throw
    fetch nil
    mov this, 0
#
    try
        throw
    catch nil
    proc myFunction
        $mymacro
        halt 0
    end
jmp 345lmao
    thread lma29837982743o -> {
        call myFunction
    }
:345lmao
    ;try

    jmp 35j3452l43j52kj3452453
    thread Funnyx -> {
        __say 0, "Is this me going crazy? DBG 0"
        fetch testinggg
        __say 0, "Is this me going crazy? DBG 1"
        ;lock &testinggg
        mov this, "Testing the funny string"
        __say 0, "Is this me going crazy? DBG 2"
        mov tlr, *this
        call std::ios::writeln ; test2
        ;unlock this
    }

    thread LmaoWhatsThos -> {
        __say 0, "This is the problem! DBG 1"
        nop
        zero tlr
        mov tlr, "Hello from lmaowhatsthos"
        __say 0, "This is the problem! DBG 2"

        call std::ios::writeln ; test3
        __say 0, "This is the problem! DBG 3"

    }
    :35j3452l43j52kj3452453

    thread lmao2734273 -> {
        fetch testinggg
        lock this
        mov this,"xd\n"
        mov tlr, *this
        call std::ios::writeln
        unlock this
    }
    ./lmaoo
        proc hix
            __say 0, "This is the problem! DBG 4"

            mov tlr, "hi man bro lol\n"
            mov fdx, 1
            __say 0, "This is the problem! DBG 5"

            sysenter "ios"
            __say 0, "This is the problem! DBG 6"
            syscall
            __say 0, "This is the problem! DBG 7"
            halt 0
        end

        
        thread Interesting -> {
            __say 0, "This is the problem! DBG 8"
            call lmaoo::hix
            __say 0, "This is the problem! DBG 9"
        }
    ./!lmaoo

    fetch testinggg
    lock this
    mov this, "Testing the funny string from main thread"
    mov tlr, *this
    call std::ios::writeln ; test
    unlock this

jmp 2387ztdlfgkj
    await &Funnyx
    mov tlr, &Funnyx
    sysenter "thread"
    mov fdx, 1
    syscall

    await &LmaoWhatsThos
    mov tlr, &LmaoWhatsThos
    syscall
:2387ztdlfgkj
    async &TEST
    await &TEST
    mov fdx, 1
    sysenter "thread"
    mov tlr, &TEST
    syscall

    await &lmaoo::Interesting
    mov tlr, &lmaoo::Interesting
    syscall

    ;call myFunction
    ;catch nil
    try
        callc idkman
        fetch nil
        mov this, 3
        nop
    catch kids
:lmaoooo 



    
    zero rax
    mov rax, 223 ; exit code
    ret *rax ; returns from the whole program to the host OS or newasm shell
; -------------------------- FUNCTIONS -------------------------- ;
:idkman
    ;try
        nop
    ;catch nil
    retc
:kids
    try
        ./lmao
        ./!lmao
        ./lmao
        ./!lmao
        ./lmao
        ./!lmao
        nop
    catch nil
    mov tlr, "Error properly catched and handled, exit code: "
    call std::ios::write
    mov tlr, *rax
    call std::ios::writeln
    jmp lmaoooo
@mangle ; tell the compiler to do compile-time mangling of the label
./std ; these namespace delcarations don't exist in the binary because they're under @mangle
./345345
:FkinFunction
; you don't have to use ./!std since it is not in the runtime, but pure compiler information
    zero tlr ; this is gonna get removed
    mov tlr, 3 ; this is gonna get removed by the compiler as well
    mov tlr, "Hello from func"
    call this
    retc ; returns from function

:Loop
    cmp rax, 0
    jz newasm::lmao::endLoop
    dec rax
    mov tlr, "Hello from Loop"
    call this
    callc Loop ; we got recursion in newasm finally!
@mangle
./newasm
./lmao
:endLoop
    retc

    ;using "kuku"
using "ios"
.data
    ./std
        string myText2: "hi"
        intg sus: 1
        float lol: 3
        char xd: 3
    ./!std
.start
    mov dlx, */helloworld ; are super hidden
    mov tlr, std::myText2
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall
    ret 0

    thread myAsyncCode -> {

    }

    class myClass -> { ; potential idea

    }

    proc myFunction -> {

    }

    movasx movaddr movas mov vmov sigmas
    xd test1 test2

.data
    intg array_size: 100
    intg[] array_wow: [100] ; not yet in the language
    tuple boom: ()
    intg i: 0
    string[array_size] how_tO_make_ThIS: nil

    intg myNumber: nil->myTag
.start
    mov tlr, array_wow[0]
    lea &boom, 1
    mov &boom, 2
:loop
    cmp i, 100
    jz outside
    ;do smth w 0

    mov cr0, i
    mov cr1, 1
    add
    mov &i, *cr0

    jmp loop
:outside