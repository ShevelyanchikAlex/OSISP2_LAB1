#ifndef PROJ1_RECT_H
#define PROJ1_RECT_H


#include <windows.h>

class Rect {
public:
    RECT rect{};
    int x{}, y{};
    int height{}, width{};
    double vx{}, vy{};

    Rect(int x, int y, int height, int width, double vx, double vy);

    Rect();

    void paintRect(HWND hwnd) const;
};


#endif //PROJ1_RECT_H
