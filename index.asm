;ok
.start
    align 3
    cls
:startofprog
using "ios"
using "fs"
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
        
        mov tlr, "Hello from thread"
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

        mov tlr, "hello from thread 2"
        mov bos, 5
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
    }
    thread  testthread3 -> {

        mov tlr, "hello from thread 3"
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
    int 0x1 ; realloc

    mov tlr, 1
    int 0x2

    jmp label3543 ; skip diabolical error check below
    mov tlr, yes
    mov stl, 0c1
    mov fdx, 1
    sysenter "ios"
    syscall
:label3543
    ;mipazuzuzu
.hndl
    ~exit = __exit_proc
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

    sysreq  data , &return_vals

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

    push  some#randomcrap
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
.hndl
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
    load *, 736.38 ; hea = something
    load &, &testdecimal ; myvar = hea

    free nil ; be responsible

    mov  tlr , testdecimal
    mov  stl , 0c1
    mov  fdx , 2
    syscall 

    ; Allocate more space:
    malloc 4
    mov hea, [0]
    load *, 9821.38 ; hea : 63
    load &, &testfloat2 ; myvar : hea
    
    mov  tlr , testfloat2
    mov stl , 0c1
    mov  fdx , 2
    syscall  

    mov tlr, *hea
    
    ;mov  hea , 0 ; manually access the first address ----------> very bad idea
    mov hea, [0]
    heap 4
    heap -4 ; for testing
    load  & , &testfloat2 ; myvar : hea
    free nil
    
    mov  tlr , testfloat2
    mov  stl , 0c1
    mov  fdx , 2
    syscall  

    ; ANOTHER BAD IDEA mov  hea , 1 ; manually access the second address
    malloc 4
    mov hea, [0]
    load &, &testfloat2 ; myvar : hea
    mov  tlr , testfloat2
    mov  stl , 0c1
    mov  fdx , 2
    syscall
    free nil

    ;heap -1 ; let all the memory go VERY UNACCURATE

    stor  prp , &testreference
    mov  tlr , testreference
    mov  fdx , 6
    mov  stl , 0c1
    syscall   ; test2342343

    mov  prp , &testreference
    stor  prp , &testreference
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

    mov &exptest, *cr0
    mov  tlr , exptest
    mov  fdx , 2
    mov  stl , 0c1
    syscall  

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

    mov prp , &halttest
    mov stl , 0c1
    mov fdx , 6

    dec  prp
.data
    ref  prptest : &PROCEDURETEST
    ref  uninref: nil
    ref reference74298374: nil
    float  logtest : 0.0
.start
    stor   prp , &prptest
    mov tlr, prptest
    sysenter "ios"
    syscall 


    mov   cr0 , 8.0
    mov   cr1 , 2.0

    log

    mov &logtest, *cr0
    mov   tlr , logtest
    mov   stl , 0c1
    mov   fdx , 2
    sysenter "ios"
    syscall   

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
        ref  reference : &prptest
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

    sysenter "fs"
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
    load *, 'p'
    load &, &character
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

    proc  __exit_proc
        db tlr
        mov   tlr , "exit proc called"
        mov   stl , 0c1
        ;mov bos, 100
        mov   fdx , 1
        sysenter "ios"
        syscall   
        halt 0
    end

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
    load *, "OhMy"
    load &, &testText
    free nil
    mov tlr, testText
    db tlr
    mov stl, 0c1
    int 0x3
    mov bos, 4
    mov fdx, 1
    sysenter "ios"
    syscall
    int 0x3
    wait 2000
.hndl
    0x827 = procedurename ;assign a hex code to a procedure
.start
    proc procedurename
        mov tlr, <0> ;get the first argument
        mov stl, 0c1
        mov fdx, <1> ;get the second argument
        sysenter "ios"
        syscall
        halt 0
    end
    db stk

    push 1 ; push the sec arg
    push "call stack works" ; push the first arg
    push 0x827 ; call the procedure
    stack ;clear up the stack after the procedure call
    wait 3000
    db stk
    push 1 ; push the sec arg
    push "call stack works again!" ; push the first arg
    push 0x827 ; call the procedure
    stack ;clear up the stack after the procedure call


.hndl
    0x00 = thisisanerror
.start
    db stk
    proc thisisanerror
        __say 0,"works"
        halt 0
    end
    push  0x00
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
    out 1
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
    vmov {100}, "Hello World from virtual memory"
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

    mov dlx, "testlib"
    mov fdx, 1
    sysenter "ext"
    syscall
using "ios"
.data
    [lock]
    string myLockedstr : "Locked"
    obj lockedObj : {
        string lockedMember : "LockedMember"
    }
    [!lock]
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
        recv &threads::lol::myChannel
        mov stl, 0c1
        mov fdx, 1
        sysenter "ios"
        syscall
    }

    send &threads::lol::myChannel, "Data from the channel"
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
        load *, "Hello from crazy var!"
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
        load *, 72345
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
    load *, 738
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
.hndl
    0xff = std::testproc
    0xff3 = lol::writexd
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
    push 0xff
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
        proc writexd
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

    push 3
    push 0xff3
    stack

    push "hello"
    push 0xff3
    stack
:lol24332243
    mov tlr, 1
    call std::ios::writeln

    mov tlr, "Hello from stdlib"
    call std::ios::writeln

    push "Hello"
    pop nil

.data
    ./test
        intg alloc: 0
        intg var: 0
    ./!test
.start
    malloc 12
        mov &test::alloc, *tlr ; assign a pointer to a variable, so we can free it later
        mov hea, [0]
        load *, 73837
        del &test::var
        movaddr &test::var, *hea

        mov tlr, test::var
        call std::ios::writeln
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
    call std::ios::writeln
    int 0x3

    mov tlr, * hea
    call std::ios::writeln

	;mov tlr, ui::29042384_w

.data
    union myUnion: nil
    union myUnion2: "Hello"
    intg random: 0
    tuple testTupleOh: (0, 543.534, "oh my God")
.start
    movas intg

    mov tlr, myUnion
    call std::ios::writeln

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
    call std::ios::writeln

    mov tlr, #myUnion
    call std::ios::writeln

    mov tlr, #student
    call std::ios::writeln

    mov tlr, #testTupleOh
    call std::ios::writeln

.data
    string testStringBro: "Hello"
    intg myInt: 29083
    string testStringBro2: "Hello"
    intg myInt2: 298374537

    string TetsSTR: "/"

    intg alloc2: 0
.start
    mov tlr, "Address test:"
    call std::ios::writeln

    mov tlr, #testStringBro
    call std::ios::writeln

    mov tlr, #myInt
    call std::ios::writeln

    mov tlr, #testStringBro2
    call std::ios::writeln

    mov tlr, #myInt2
    call std::ios::writeln

    mov tlr, "Malloc test"
    call std::ios::writeln

    malloc 55
    mov &alloc2, *tlr
    mov hea, [0]
    load *, 92873
    sel alloc2 ; select the shi
    call std::ios::writeln
    mov hea, [0]
    mov tlr, *hea
    call std::ios::writeln
    load &, &myInt2
    mov tlr, myInt2
    call std::ios::writeln
    free alloc2

    movas intg
    del &myUnion
    mov &myUnion, 39 ; memory leak if we don't delete it!

    push "I am a sigma, boy"
    pop &TetsSTR

    mov tlr, TetsSTR
    call std::ios::writeln
.data
    ctx myContext: () ; beta feature
    ./test
        ctx myContext2: ()
    ./!test
.start
    mov &myContext, ("key": 1.67, "key2": 843, "lmfao": "ja sam degen")
    mov tlr, myContext("key")
    call std::ios::writeln
    mov tlr, myContext("lmfao")
    call std::ios::writeln

    mov &myContext, ("hehe":98)

    mov tlr, myContext("hehe")
    call std::ios::writeln

    mov &test::myContext2, ("yay":"yay called!")
    mov tlr, test::myContext2("yay")
    call std::ios::writeln

    mov &test::myContext2, ()

    merge &test::myContext2, ("yay": 23)

    mov tlr, test::myContext2("yay")
    call std::ios::writeln

    merge &myContext, ("hehe" : nil, "kjdhf": "VEOMA KORISNA STVAR JBG") ; hehe will be deleted cuz we set it to nil
    mov tlr, myContext("kjdhf")
    call std::ios::writeln

    merge &myContext, ("hehe": 387) ; fix invalid memacc -> we give it a new value
    mov tlr, myContext("hehe") ; <- invalid memory access error 
    call std::ios::writeln

    merge &test::myContext2, ("test" : 243)
    mov tlr, test::myContext2("test")
    call std::ios::writeln

    merge &test::myContext2, ("test": "very cool")
    mov tlr, test::myContext2("test")
    call std::ios::writeln

    merge &test::myContext2, ("test": nil, "test": "very cool again") ; this should work again!
    mov tlr, test::myContext2("test")
    call std::ios::writeln
.data
    [transient]
        intg mynum: 43
    [!transient]
.start
    mov tlr, mynum
    call std::ios::writeln
.data
    intg mynum2: 29843
.start
    mov tlr, mynum ; will display 29843
    call std::ios::writeln

    ;idk
    mov tlr, "Hey"
    mov stl, 0c0

link "testfile.asm" ; link a file

    using "ctx"
    mov tlr, &test::myContext2
    mov fdx, 1
    sysenter "ctx"
    syscall

    call std::ios::writeln

    link "testfile3.asm"
    jmp hehe
    :lel__________
    mov tlr, "labele rade fino Allaha mi"
    call std::ios::writeln
    ;LMFAO TEST
:hehe

    mov tlr, 38356
    mov stl, -25
    call std::math::addition
    call std::ios::writeln

    mov tlr, 38356
    mov stl, -25
    call std::math::subtraction
    call std::ios::writeln
.data
    ./hi
        intg age: 19
        tuple humanData: ("ele bre", "nigga")
        ctx myContext3: ("some_data": "lel")
    ./!hi
.start
    mov tlr, "Hello, I am {hi::humanData(0)} {hi::humanData(1)}, I am {hi::age} {hi::myContext3("some_data")}"
    sysenter "txtop"
    mov fdx, 4
    syscall

    call std::ios::writeln

    mov tlr, 223
    ret *tlr
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