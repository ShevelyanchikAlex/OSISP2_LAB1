#include <windows.h>
#include "window/Window.h"
#include <cmath>
#include "rect/Rect.h"
#include "const.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void create_combobox(HWND hwnd);

void mousewheel_handling(WPARAM wParam);

void keydown_handling(WPARAM wparam);

void auto_mode_handling(HWND hwnd);

Rect rect;
byte selected_mode = FIRST_MODE_VAL;

int WinMain(HINSTANCE hinstance, HINSTANCE, LPTSTR, int mode) {
    HWND hwnd = nullptr;
    MSG msg;
    WNDCLASS wcl;

    Window window = Window(hwnd, msg, wcl);
    window.reg_window(hinstance, "Window", WndProc);
    window.create_window(hinstance);
    window.show_window();
    UpdateWindow(hwnd);

    while (GetMessage(&window.msg, nullptr, 0, 0)) {
        TranslateMessage(&window.msg);
        DispatchMessage(&window.msg);
    }
    return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            create_combobox(hwnd);
            rect = Rect(RECT_START_X, RECT_START_Y, RECT_HEIGHT, RECT_WIDTH, RECT_VX, RECT_VY);
            SetTimer(hwnd, 1, 25, nullptr);
            break;
        case WM_KEYDOWN:
            keydown_handling(wParam);
            break;
        case WM_MOUSEWHEEL:
            if (selected_mode == FIRST_MODE_VAL) {
                mousewheel_handling(wParam);
            }
            break;
        case WM_MOUSEMOVE:
            if (selected_mode == SECOND_MODE_VAL) {
                rect.x = LOWORD(lParam);
                rect.y = HIWORD(lParam);
            }
            break;
        case WM_TIMER: {
            InvalidateRect(hwnd, &rect.rect, true);
            if (selected_mode == THIRD_MODE_VAL) {
                auto_mode_handling(hwnd);
            }
            rect.rect.left = rect.x - rect.width - 1;
            rect.rect.top = rect.y - rect.height - 1;
            rect.rect.right = rect.x + rect.width + 1;
            rect.rect.bottom = rect.y + rect.height + 1;
            InvalidateRect(hwnd, &rect.rect, false);
        }
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == CB_ID) {
                if (HIWORD(wParam) == CBN_SELCHANGE) {
                    selected_mode = SendMessage((HWND) lParam, CB_GETCURSEL, 0, 0);
                }
            }
            break;
        case WM_PAINT:
            rect.paint_rect(hwnd);
            break;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void create_combobox(HWND hwnd) {
    HWND combo_box = CreateWindow("combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 0, 0, CB_WIDTH, CB_HEIGHT,
                                  hwnd, (HMENU) CB_ID, nullptr, nullptr);
    SendMessageA(combo_box, CB_ADDSTRING, 0, (LPARAM) FIRST_MODE);
    SendMessageA(combo_box, CB_ADDSTRING, 0, (LPARAM) SECOND_MODE);
    SendMessageA(combo_box, CB_ADDSTRING, 0, (LPARAM) THIRD_MODE);
    SendMessageA(combo_box, CB_ADDSTRING, 0, (LPARAM) FOURTH_MODE);
    SendMessage(combo_box, CB_SETCURSEL, 0, 0);
}


void auto_mode_handling(HWND hwnd) {
    rect.x += (int) rect.vx;
    rect.y += (int) rect.vy;

    RECT rt;
    GetClientRect(hwnd, &rt);
    int right_wall = rt.right;
    int bottom_wall = rt.bottom;
    if (rect.x >= right_wall - rect.width) rect.vx = -std::abs(rect.vx);
    if (rect.y > bottom_wall - rect.height) rect.vy = -std::abs(rect.vy);
    if (rect.x < 0) rect.vx = std::abs(rect.vx);
    if (rect.y < 0) rect.vy = std::abs(rect.vy);
}

void keydown_handling(WPARAM wparam) {
    switch (wparam) {
        case VK_LEFT:
        case 0x41:
            rect.x -= (int) (rect.vx * RECT_DELTA_SPEED);
            break;
        case VK_RIGHT:
        case 0x44:
            rect.x += (int) (rect.vx * RECT_DELTA_SPEED);
            break;
        case VK_UP:
        case 0x57:
            rect.y -= (int) (rect.vy * RECT_DELTA_SPEED);
            break;
        case VK_DOWN:
        case 0x53:
            rect.y += (int) (rect.vy * RECT_DELTA_SPEED);
            break;
        default:
            break;
    }
}


void mousewheel_handling(WPARAM wParam) {
    byte sign = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1 : -1;
    int delta = GET_WHEEL_DELTA_WPARAM(wParam) / 10;

    if (LOWORD(wParam) == MK_SHIFT) {
        if (sign > 0) {
            rect.x -= delta;
        } else {
            rect.x += delta;
        }
    } else {
        if (sign > 0) {
            rect.y -= delta;
        } else {
            rect.y += delta;
        }
    }
}
