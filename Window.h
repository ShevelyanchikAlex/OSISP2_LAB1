#include <windows.h>

#ifndef PROJ1_WINDOW_H
#define PROJ1_WINDOW_H


class Window {
public:
    HWND hwnd;// Дескриптор главного окна
    MSG msg{};
    WNDCLASS wcl{};

    Window(HWND hwnd, MSG msg, WNDCLASS wcl);

    bool reg_window(HINSTANCE hinstance, LPCSTR lpszClassName, WNDPROC WndProc);

    void create_window(HINSTANCE hinstance);

    void show_window() const;
};


#endif //PROJ1_WINDOW_H
