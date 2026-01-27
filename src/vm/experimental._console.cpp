#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

//////////////////////////////////////////////////////////////
//                    GLOBAL STATE
//////////////////////////////////////////////////////////////

static HWND g_hwnd = nullptr;
static HWND g_edit = nullptr;

static std::vector<std::string> g_lines;
static std::thread g_uiThread;
static std::atomic<bool> g_running = false;

static std::mutex g_mutex;
static std::condition_variable g_inputCV;

static std::string g_inputResult;
static bool g_waitingForInput = false;

//////////////////////////////////////////////////////////////
//              EDIT SUBCLASS (ENTER HANDLER)
//////////////////////////////////////////////////////////////

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYDOWN && wParam == VK_RETURN)
    {
        char buffer[512];
        GetWindowTextA(hwnd, buffer, sizeof(buffer));

        {
            std::lock_guard<std::mutex> lock(g_mutex);
            g_inputResult = buffer;
            g_lines.push_back("> " + g_inputResult);
            g_waitingForInput = false;
        }

        SetWindowTextA(hwnd, "");
        if (g_hwnd)
            InvalidateRect(g_hwnd, nullptr, TRUE);
        g_inputCV.notify_one();

        return 0; // POJELI ENTER
    }

    return CallWindowProc(
        (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA),
        hwnd, msg, wParam, lParam
    );
}

//////////////////////////////////////////////////////////////
//                  WINDOW PROCEDURE
//////////////////////////////////////////////////////////////

LRESULT CALLBACK MiniWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int scrollPos = 0;

    switch (msg)
    {
        case WM_VSCROLL:
        {
            int nScrollCode = LOWORD(wParam);
            int nPos = HIWORD(wParam);

            SCROLLINFO si = {};
            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;
            GetScrollInfo(hwnd, SB_VERT, &si);

            switch (nScrollCode)
            {
                case SB_LINEUP:     si.nPos -= 1; break;
                case SB_LINEDOWN:   si.nPos += 1; break;
                case SB_PAGEUP:     si.nPos -= si.nPage; break;
                case SB_PAGEDOWN:   si.nPos += si.nPage; break;
                case SB_THUMBTRACK: si.nPos = nPos; break;
            }

            if (si.nPos < si.nMin) si.nPos = si.nMin;
            if (si.nPos > si.nMax - (int)si.nPage + 1) si.nPos = si.nMax - si.nPage + 1;

            si.fMask = SIF_POS;
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
            scrollPos = si.nPos;
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rc;
            GetClientRect(hwnd, &rc);

            int y = 10 - scrollPos * 18;
            std::lock_guard<std::mutex> lock(g_mutex);

            for (const auto& line : g_lines)
            {
                TextOutA(hdc, 10, y, line.c_str(), (int)line.size());
                y += 18;
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE:
        {
            RECT rc;
            GetClientRect(hwnd, &rc);

            SCROLLINFO si = {};
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = (int)g_lines.size();
            si.nPage = (rc.bottom - rc.top) / 18;
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//////////////////////////////////////////////////////////////
//                  UI THREAD
//////////////////////////////////////////////////////////////

void uiThreadFunc(const char* title, int w, int h)
{
    HINSTANCE hInst = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc   = MiniWndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "MiniTextWindowClass";

    RegisterClass(&wc);

    g_hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        CW_USEDEFAULT, CW_USEDEFAULT,
        w, h,
        nullptr, nullptr,
        hInst, nullptr
    );

    g_edit = CreateWindowEx(
        0,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, h - 70, w - 40, 25,
        g_hwnd,
        nullptr,
        hInst,
        nullptr
    );

    // SUBCLASS EDIT CONTROL
    SetWindowLongPtr(
        g_edit,
        GWLP_USERDATA,
        (LONG_PTR)SetWindowLongPtr(g_edit, GWLP_WNDPROC, (LONG_PTR)EditProc)
    );

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    g_hwnd = nullptr;
}

//////////////////////////////////////////////////////////////
//                   JAVNI API
//////////////////////////////////////////////////////////////

void openWindow(const char* title = "Mini Window", int width = 500, int height = 400)
{
    if (g_running) return;

    g_running = true;
    g_uiThread = std::thread(uiThreadFunc, title, width, height);
}

void printToWindow(const std::string& text)
{
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_lines.push_back(text);
    }

    if (g_hwnd)
    {
        // update scroll bar range
        RECT rc;
        GetClientRect(g_hwnd, &rc);
        SCROLLINFO si = {};
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        si.nMin = 0;
        si.nMax = (int)g_lines.size();
        si.nPage = (rc.bottom - rc.top) / 18;
        si.nPos = si.nMax - si.nPage;
        if (si.nPos < 0) si.nPos = 0;
        SetScrollInfo(g_hwnd, SB_VERT, &si, TRUE);

        InvalidateRect(g_hwnd, nullptr, TRUE);
    }
}

std::string inputTextToWindow()
{
    std::unique_lock<std::mutex> lock(g_mutex);
    g_waitingForInput = true;

    g_inputCV.wait(lock, [] { return !g_waitingForInput; });
    return g_inputResult;
}

void closeWindow()
{
    if (!g_running) return;

    g_running = false;

    if (g_hwnd)
        PostMessage(g_hwnd, WM_CLOSE, 0, 0);

    if (g_uiThread.joinable())
        g_uiThread.join();
}
