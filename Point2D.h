#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

class PrimitiveRenderer;

class Point2D {
private:
    float x, y;

public:
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}

    float getX() const { return x; }
    float getY() const { return y; }

    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

    void draw(PrimitiveRenderer* renderer, float point_size = 3.0f) const;

    //Przeksztalcenia geometryczne

    void translate(float dx, float dy) {
        x += dx;
        y += dy;
    }

    void scale(float sx, float sy) {
        x *= sx;
        y *= sy;
    }

    void rotate(float angle_deg, float cx = 0.0f, float cy = 0.0f) {
        float rad = angle_deg * 3.1415926535f / 180.0f;
        float s = sin(rad);
        float c = cos(rad);

        float tx = x - cx;
        float ty = y - cy;

        float x_new = tx * c - ty * s;
        float y_new = tx * s + ty * c;

        x = x_new + cx;
        y = y_new + cy;
    }
};

#endif
