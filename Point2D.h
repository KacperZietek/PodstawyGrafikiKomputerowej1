#ifndef POINT2D_H
#define POINT2D_H

#include <allegro5/allegro_primitives.h>

class Point2D {
private:
    float x, y;

public:
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}

    float getX() const { return x; }
    float getY() const { return y; }
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }

    //metoda do rysowania punktu
    void draw(ALLEGRO_COLOR color) const {
        al_draw_filled_circle(x, y, 2.0f, color);
    }
};

#endif
