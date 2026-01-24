package main

/*
#include <stdlib.h>
*/
import "C"

import (
	"image/color"
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

func main() {}
