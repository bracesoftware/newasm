// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

package main
/*
#include <stdlib.h>
*/
import "C"

import (
    //for networking
    "fmt"
    "net"
    "net/http"
    "io"
    "strings"
    "unsafe"
    //for crypto
    "crypto/sha256"
    "encoding/hex"
)
/*
//export send_tcp
func send_tcp(addr *C.char, msg *C.char) C.int {
    goAddr := C.GoString(addr)
    goMsg := C.GoString(msg)

    conn, err := net.Dial("tcp", goAddr)
    if err != nil {
        fmt.Println("NewASM GO :: Connection error -> ", err)
        return -1
    }
    defer conn.Close()

    _, err = conn.Write([]byte(goMsg))
    if err != nil {
        fmt.Println("NewASM GO :: Write error -> ", err)
        return -2
    }

    return 0
}
*/
//export send_tcp
func send_tcp(addr *C.char, msg *C.char, errBuf *C.char, bufSize C.int) C.int {
    goAddr := C.GoString(addr)
    goMsg := C.GoString(msg)

    writeError := func(e error) {
        if errBuf != nil && bufSize > 0 {
            errStr := e.Error()
            n := int(bufSize) - 1
            if len(errStr) < n {
                n = len(errStr)
            }
            
            p := unsafe.Pointer(errBuf)
            store := (*[1 << 30]byte)(p)[:n:n]
            copy(store, errStr)
            
            *(*byte)(unsafe.Pointer(uintptr(p) + uintptr(n))) = 0
        }
    }

    conn, err := net.Dial("tcp", goAddr)
    if err != nil {
        writeError(err)
        return -1
    }
    defer conn.Close()

    _, err = conn.Write([]byte(goMsg))
    if err != nil {
        writeError(err)
        return -2
    }

    return 0
}

//export recv_tcp
func recv_tcp(addr *C.char) *C.char {
    goAddr := C.GoString(addr)

    conn, err := net.Dial("tcp", goAddr)
    if err != nil {
        return C.CString(fmt.Sprintf("Dial error -> %v", err))
    }
    defer conn.Close()

    buf := make([]byte, 1024)
    n, err := conn.Read(buf)
    if err != nil {
        return C.CString(fmt.Sprintf("Read error -> %v", err))
    }

    return C.CString(string(buf[:n]))
}

//export free_string
func free_string(str *C.char) {
    C.free(unsafe.Pointer(str))
}


//export tcp_init
func tcp_init() {
	fmt.Println("NewASM GO :: Successfully loaded the netwoking system.")
}

func main() {}


/*
HTTP FUNCTIONS
*/

//export http_get
func http_get(url *C.char) *C.char {
    goURL := C.GoString(url)

    resp, err := http.Get(goURL)
    if err != nil {
        return C.CString(fmt.Sprintf("HTTP GET error -> %v", err))
    }
    defer resp.Body.Close()

    body, err := io.ReadAll(resp.Body)
    if err != nil {
        return C.CString(fmt.Sprintf("Read body error -> %v", err))
    }

    return C.CString(string(body))
}

//export http_post
func http_post(url *C.char, data *C.char) *C.char {
    goURL := C.GoString(url)
    goData := C.GoString(data)

    resp, err := http.Post(goURL, "application/x-www-form-urlencoded", strings.NewReader(goData))
    if err != nil {
        return C.CString(fmt.Sprintf("HTTP POST error -> %v", err))
    }
    defer resp.Body.Close()

    body, err := io.ReadAll(resp.Body)
    if err != nil {
        return C.CString(fmt.Sprintf("Read body error -> %v", err))
    }

    return C.CString(string(body))
}

//crypto shi

//export crypto_sha256_hex
func crypto_sha256_hex(data *C.char) *C.char {
	goData := C.GoString(data)
	hash := sha256.Sum256([]byte(goData))
	out := hex.EncodeToString(hash[:])
	return C.CString(out)
}
