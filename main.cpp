#include <windows.h>
#include "window/Window.h"
#include <cmath>
#include "rect/Rect.h"
#include "const.h"


RECT windowSpace;
Rect rect;
byte selected_mode = FIRST_MODE_VAL;
PAINTSTRUCT ps;
HDC memDC;
HBITMAP bmp, memBmp, oldBitmap;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void create_mode_combobox(HWND &hwnd);
void create_obj_combobox(HWND &hwnd);
void handle_mousewheel(WPARAM &wParam);
void handle_keydown(WPARAM &wparam, HWND &hwnd);
void handle_auto_mode(HWND &hwnd);
void check_zone_validation(HWND &hwnd);
void load_image_bmp(HWND &hwnd);
void draw_image(HDC &hdc);
void paint_image(HWND &hwnd);
int calc_offset(byte sign, int coord, int delta);
void handle_wm_command(HWND &hwnd, WPARAM wParam, LPARAM lParam);
void track_coord();


int WinMain(HINSTANCE hinstance, HINSTANCE, LPTSTR, int mode) {
    Window window = Window();
    window.reg_window(hinstance, "Window", WndProc);
    window.create_window(hinstance);
    window.show_window();

    while (GetMessage(&window.msg, nullptr, 0, 0)) {
        TranslateMessage(&window.msg);
        DispatchMessage(&window.msg);
    }
    return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            rect = Rect(RECT_START_X, RECT_START_Y, RECT_HEIGHT, RECT_WIDTH, RECT_VX, RECT_VY);
            create_mode_combobox(hwnd);
            create_obj_combobox(hwnd);
            SetTimer(hwnd, 1, 25, nullptr);
            break;
        case WM_KEYDOWN:
            handle_keydown(wParam, hwnd);
            break;
        case WM_MOUSEWHEEL:
            if (selected_mode == FIRST_MODE_VAL) {
                handle_mousewheel(wParam);
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
                handle_auto_mode(hwnd);
            }
            track_coord();
            InvalidateRect(hwnd, &rect.rect, false);
        }
            break;
        case WM_COMMAND:
            handle_wm_command(hwnd,wParam,lParam);
            break;
        case WM_PAINT:
            if (!bmp) {
                rect.paint_rect(hwnd);
            } else {
                paint_image(hwnd);
            }
            UpdateWindow(hwnd);
            break;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


void create_mode_combobox(HWND &hwnd) {
    HWND mode_combo_box = CreateWindow("combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 0, 0, CB_MODE_WIDTH,
                                       CB_MODE_HEIGHT, hwnd, (HMENU) CB_MODE_ID, nullptr, nullptr);
    SendMessageA(mode_combo_box, CB_ADDSTRING, 0, (LPARAM) FIRST_MODE);
    SendMessageA(mode_combo_box, CB_ADDSTRING, 0, (LPARAM) SECOND_MODE);
    SendMessageA(mode_combo_box, CB_ADDSTRING, 0, (LPARAM) THIRD_MODE);
    SendMessage(mode_combo_box, CB_SETCURSEL, 0, 0);
}

void create_obj_combobox(HWND &hwnd) {
    HWND obj_combo_box = CreateWindow("combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, CB_MODE_WIDTH, 0,
                                      CB_OBJ_WIDTH,
                                      CB_OBJ_HEIGHT, hwnd, (HMENU) CB_OBJ_ID, nullptr, nullptr);
    SendMessageA(obj_combo_box, CB_ADDSTRING, 0, (LPARAM) FIRST_OBJ);
    SendMessageA(obj_combo_box, CB_ADDSTRING, 0, (LPARAM) SECOND_OBJ);
    SendMessage(obj_combo_box, CB_SETCURSEL, 0, 0);
}

void handle_wm_command(HWND &hwnd, WPARAM wParam, LPARAM lParam){
    switch (LOWORD(wParam)) {
        case CB_MODE_ID:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                selected_mode = SendMessage((HWND) lParam, CB_GETCURSEL, 0, 0);
            }
            break;
            case CB_OBJ_ID:
                if (HIWORD(wParam) == CBN_SELCHANGE) {
                    if (!bmp) {
                        load_image_bmp(hwnd);
                    } else {
                        bmp = nullptr;
                    }
                }
                break;
    }
}



void track_coord() {
    rect.rect.left = rect.x - rect.width;
    rect.rect.top = rect.y - rect.height;
    rect.rect.right = rect.x + rect.width;
    rect.rect.bottom = rect.y + rect.height;
}

void handle_auto_mode(HWND &hwnd) {
    rect.x += (int) rect.vx;
    rect.y += (int) rect.vy;

    check_zone_validation(hwnd);
    UpdateWindow(hwnd);
}

void check_zone_validation(HWND &hwnd) {
    GetClientRect(hwnd, &windowSpace);
    if (rect.x >= windowSpace.right - rect.width) rect.vx = -std::abs(rect.vx);
    if (rect.y > windowSpace.bottom - rect.height) rect.vy = -std::abs(rect.vy);
    if (rect.x < 0) rect.vx = std::abs(rect.vx);
    if (rect.y < 0) rect.vy = std::abs(rect.vy);
}

void handle_keydown(WPARAM &wparam, HWND &hwnd) {
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


void handle_mousewheel(WPARAM &wParam) {
    byte sign = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1 : -1;
    int delta = GET_WHEEL_DELTA_WPARAM(wParam) / 10;

    if (LOWORD(wParam) == MK_SHIFT) {
        rect.x = calc_offset(sign, rect.x, delta);
    } else {
        rect.y = calc_offset(sign, rect.y, delta);
    }
}

int calc_offset(byte sign, int coord, int delta) {
    return sign > 0 ? coord -= delta : coord += delta;
}

void load_image_bmp(HWND &hwnd) {
    if ((bmp = (HBITMAP) LoadImage(nullptr,
     "C:/Users/aleksandrsevelancik/CLionProjects/OSISP2_LAB1/assets/images/image.bmp", IMAGE_BITMAP, 0,
                                   0, LR_LOADFROMFILE)) == nullptr) {
        MessageBox(nullptr, "Error", "Error to loading image!", MB_OK);
    } else {
        BITMAP bitmap;
        GetObject(bmp, sizeof(bitmap), &bitmap);
        UpdateWindow(hwnd);
        CoUninitialize();
    }
}

void paint_image(HWND &hwnd) {
    GetClientRect(hwnd, &windowSpace);
    HDC hdc;
    hdc = BeginPaint(hwnd, &ps);
    memDC = CreateCompatibleDC(hdc);
    memBmp = CreateCompatibleBitmap(hdc, windowSpace.right - windowSpace.left,
                                    windowSpace.bottom - windowSpace.top);
    oldBitmap = (HBITMAP) SelectObject(memDC, memBmp);
    FillRect(memDC, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
    draw_image(memDC);
    BitBlt(hdc, windowSpace.left, windowSpace.top, windowSpace.right - windowSpace.left,
           windowSpace.bottom - windowSpace.top, memDC,
           windowSpace.left,
           windowSpace.top, SRCCOPY);

    DeleteDC(memDC);
    DeleteObject(memBmp);
    DeleteObject(oldBitmap);
    EndPaint(hwnd, &ps);
}

void draw_image(HDC &hdc) {
    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldBitmap;

    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, bmp);

    GetObject(bmp, sizeof(bitmap), &bitmap);
    BitBlt(hdc, rect.x, rect.y, rect.width, rect.height, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
    DeleteObject(oldBitmap);
}

