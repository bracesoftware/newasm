#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>

static HWND g_hwnd = nullptr;
static std::vector<std::string> g_lines;
static std::thread g_uiThread;
static std::atomic<bool> g_running = false;

LRESULT CALLBACK MiniWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int y = 10;
            for (const auto& line : g_lines)
            {
                TextOutA(hdc, 10, y, line.c_str(), (int)line.size());
                y += 18;
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

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
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        w, h,
        nullptr, nullptr,
        hInst, nullptr
    );

    ShowWindow(g_hwnd, SW_SHOW);

    MSG msg;
    while (g_running && GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    g_hwnd = nullptr;
}

//////////////////////////////////////////////////////////////
//                   JAVNI API
//////////////////////////////////////////////////////////////

void openWindow(const char* title = "Mini Window", int width = 400, int height = 300)
{
    if (g_running) return;

    g_running = true;
    g_uiThread = std::thread(uiThreadFunc, title, width, height);
}

void printToWindow(const std::string& text)
{
    if (!g_hwnd) return;

    g_lines.push_back(text);
    InvalidateRect(g_hwnd, nullptr, TRUE);
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
