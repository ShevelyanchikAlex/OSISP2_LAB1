#include <windows.h>
#include <tchar.h>
#include "Window.h"
#include <cmath>
#include "Rect.h"
#include "resorce.h"
#include "const.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void mousewheelHandling(WPARAM wParam);

Rect r;
HWND CB;
byte selected_mode = FIRST_MODE_VAL;

int WinMain(HINSTANCE hinstance, // Дескриптор текущего приложения
            HINSTANCE,
            LPTSTR,         // Командная строка
            int mode)       // Режим отображения окна
            {
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
            CB = CreateWindow("combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 0, 0, 400, 150, hwnd,
                              (HMENU) CB_ID,
                              nullptr, nullptr);
            SendMessageA(CB, CB_ADDSTRING, 0, (LPARAM) FIRST_MODE);
            SendMessageA(CB, CB_ADDSTRING, 0, (LPARAM) SECOND_MODE);
            SendMessageA(CB, CB_ADDSTRING, 0, (LPARAM) THIRD_MODE);
            SendMessageA(CB, CB_ADDSTRING, 0, (LPARAM) FOURTH_MODE);
            SendMessage(CB, CB_SETCURSEL, 0, 0);
            r = Rect(RECT_START_X, RECT_START_Y, RECT_HEIGHT, RECT_WIDTH, RECT_VX, RECT_VY);
            SetTimer(hwnd, 1, 25, nullptr);
            break;
            case WM_KEYDOWN:
                switch (wParam) {
                                case VK_LEFT:
                        case 0x41:
                            r.x -= (int) (r.vx * RECT_DELTA_SPEED);
                            break;
                            case VK_RIGHT:
                                case 0x44:
                                    r.x += (int) (r.vx * RECT_DELTA_SPEED);
                                    break;
                                    case VK_UP:
                                        case 0x57:
                                            r.y -= (int) (r.vy * RECT_DELTA_SPEED);
                                            break;
                                            case VK_DOWN:
                                                case 0x53:
                                                    r.y += (int) (r.vy * RECT_DELTA_SPEED);
                                                    break;
                                                    default:
                                                        break;
                }
                break;
            case WM_MOUSEWHEEL:
                if (selected_mode == FIRST_MODE_VAL) {
                    mousewheelHandling(wParam);
                }
                break;

                case WM_MOUSEMOVE:
                    if (selected_mode == SECOND_MODE_VAL) {
                        r.x = LOWORD(lParam);
                        r.y = HIWORD(lParam);
                    }
                    break;
                    case WM_TIMER: {
                        InvalidateRect(hwnd, &r.rect, true);// Стираем старое положение шарика
                        if (selected_mode == THIRD_MODE_VAL) {

                            r.x += r.vx;
                            r.y += r.vy; // новые координаты по задумке должно быть так x+=*vx;y+=*vy но компилятор ругается на умножение
                            RECT rt;
                            GetClientRect(hwnd, &rt);
                            int w = rt.right;
                            int h = rt.bottom;// длина и высота рабочей области
                            if (r.x >= w - r.width)r.vx = -std::abs(r.vx); // проверка отражения от правой стенки
                            if (r.y > h - r.height)r.vy = -std::abs(r.vy);// проверка низа
                            if (r.x < 0) r.vx = std::abs(r.vx); // проверка левой стенки
                            if (r.y < 0)r.vy = std::abs(r.vy); // проверка верха
                        }
                        r.rect.left = r.x - r.width - 1;
                        r.rect.top = r.y - r.height - 1;
                        r.rect.right = r.x + r.width + 1;
                        r.rect.bottom = r.y + r.height + 1;
                        InvalidateRect(hwnd, &r.rect, false); // нарисовать новое положение
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
                            r.paintRect(hwnd);
                            break;
                            case WM_DESTROY:
                                KillTimer(hwnd, 1);
                                PostQuitMessage(0);
                                break;
                                default:
                                    return DefWindowProc(hwnd, message, wParam, lParam);
    }
}


void mousewheelHandling(WPARAM wParam) {
    int zDelta;
    short mousewheel;
    int sign;
    mousewheel = GET_WHEEL_DELTA_WPARAM(wParam);
    zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    sign = mousewheel > 0 ? 1 : -1;
    int delta = zDelta / 10 * sign;

    if (LOWORD(wParam) == MK_SHIFT) {
        if (sign > 0) {
            r.x -= delta;
        } else {
            r.x += delta;
        }
    } else {
        if (sign > 0) {
            r.y -= delta;
        } else {
            r.y += delta;
        }
    }
}