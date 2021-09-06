#include <windows.h>
#include "Window.h"


Window::Window(HWND hwnd, MSG msg, WNDCLASS wcl) {
    this->hwnd = hwnd;
    this->msg = msg;
    this->wcl = wcl;
}

bool Window::reg_window(HINSTANCE hinstance, LPCSTR lpcstr, WNDPROC wndProc) {
    wcl.hInstance = hinstance;
    wcl.lpszClassName = lpcstr;
    wcl.lpfnWndProc = wndProc;
    wcl.style = CS_HREDRAW;
    wcl.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcl.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcl.lpszMenuName = nullptr;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    if (!RegisterClass(&wcl)) return false;
}

void Window::create_window(HINSTANCE hinstance) {
    hwnd = CreateWindow("Window", "Lab 1", WS_OVERLAPPEDWINDOW, 200, 200, 500, 450, HWND_DESKTOP, nullptr, hinstance,
                        nullptr);
}

void Window::show_window() const {
    ShowWindow(hwnd, SW_SHOW);
}


