#include <windows.h>
#include "Window.h"
#include "../const.h"


Window::Window(HWND hwnd, MSG msg, WNDCLASS wcl) {
    this->hwnd = hwnd;
    this->msg = msg;
    this->wcl = wcl;
}

Window::Window() {
    this->hwnd = HWND();
    this->msg = MSG();
    this->wcl = WNDCLASS();
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
    hwnd = CreateWindow("Window", "Lab 1", WS_OVERLAPPEDWINDOW, WINDOW_X_OFFSET, WINDOW_Y_OFFSET, WINDOW_WIDTH,
                        WINDOW_HEIGHT, HWND_DESKTOP, nullptr, hinstance, nullptr);
}

void Window::show_window() const {
    ShowWindow(hwnd, SW_SHOW);
}


