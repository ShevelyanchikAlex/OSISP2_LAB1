#include "Rect.h"

Rect::Rect(int x, int y, int height, int width, double vx, double vy) {
    this->x = x;
    this->y = y;
    this->height = height;
    this->width = width;
    this->vx = vx;
    this->vy = vy;
}

Rect::Rect() {}

void Rect::paint_rect(HWND hwnd) const {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    HBRUSH br = CreateSolidBrush(RGB(200, 50, 30));
    HBRUSH oldBr = (HBRUSH) SelectObject(hdc, br);

    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, oldBr);
    DeleteObject(br);
    DeleteObject(oldBr);
    EndPaint(hwnd, &ps);
}
