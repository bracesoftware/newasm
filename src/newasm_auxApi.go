/*

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/

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
    //for console
    "image/color"
	"sync"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
)

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

//export recv_tcp
func recv_tcp(addr *C.char) *C.char {
    goAddr := C.GoString(addr)

    conn, err := net.Dial("tcp", goAddr)
    if err != nil {
        return C.CString(fmt.Sprintf("NewASM GO :: Error -> %v", err))
    }
    defer conn.Close()

    buf := make([]byte, 1024)
    n, err := conn.Read(buf)
    if err != nil {
        return C.CString(fmt.Sprintf("NewASM GO :: Read error -> %v", err))
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
        return C.CString(fmt.Sprintf("NewASM GO :: HTTP GET error -> %v", err))
    }
    defer resp.Body.Close()

    body, err := io.ReadAll(resp.Body)
    if err != nil {
        return C.CString(fmt.Sprintf("NewASM GO :: Read body error -> %v", err))
    }

    return C.CString(string(body))
}

//export http_post
func http_post(url *C.char, data *C.char) *C.char {
    goURL := C.GoString(url)
    goData := C.GoString(data)

    resp, err := http.Post(goURL, "application/x-www-form-urlencoded", strings.NewReader(goData))
    if err != nil {
        return C.CString(fmt.Sprintf("NewASM GO :: HTTP POST error -> %v", err))
    }
    defer resp.Body.Close()

    body, err := io.ReadAll(resp.Body)
    if err != nil {
        return C.CString(fmt.Sprintf("NewASM GO :: Read body error -> %v", err))
    }

    return C.CString(string(body))
}

/*
CONSOLE FUNCTIONS
*/

var (
	a         fyne.App
	w         fyne.Window
	logText   *canvas.Text
	input     *widget.Entry
	inputChan chan string
	once      sync.Once
)

//export show_console
func show_console(title *C.char) {
	once.Do(func() {
		inputChan = make(chan string)

		a = app.New()
		w = a.NewWindow(C.GoString(title))

		logText = canvas.NewText("", color.RGBA{255, 255, 255, 255})
		logText.TextSize = 14
		logText.Alignment = fyne.TextAlignLeading

		input = widget.NewEntry()
		input.SetPlaceHolder("")

		input.OnSubmitted = func(s string) {
			logText.Text += s + "\n"
			logText.Refresh()
			inputChan <- s
			input.SetText("")
		}

		bg := canvas.NewRectangle(color.RGBA{0, 0, 0, 255})

		content := container.NewBorder(
			nil,
			input,
			nil,
			nil,
			container.NewVScroll(logText),
		)

		w.SetContent(container.NewMax(bg, content))
		w.Resize(fyne.NewSize(700, 450))

		go w.ShowAndRun()
	})
}

//export console_out
func console_out(text *C.char) {
	if logText == nil {
		return
	}
	logText.Text += C.GoString(text)
	logText.Refresh()
}

//export console_in
func console_in() *C.char {
	s := <-inputChan
	return C.CString(s)
}

//export close_console
func close_console() {
	if w != nil {
		w.Close()
	}
}
