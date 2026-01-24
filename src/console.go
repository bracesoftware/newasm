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
	"sync"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
)

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

		logText = canvas.NewText("", fyne.NewColor(255, 255, 255, 255))
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

		bg := canvas.NewRectangle(fyne.NewColor(0, 0, 0, 255))

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
	logText.Text += C.GoString(text) + "\n"
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

func main() {}
