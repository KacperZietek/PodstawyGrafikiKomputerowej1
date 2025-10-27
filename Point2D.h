#ifndef POINT2D_H
#define POINT2D_H

class PrimitiveRenderer;

class Point2D {
private:
    float x, y;

public:
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}

    // Gettery
    float getX() const { return x; }
    float getY() const { return y; }

    // Settery
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

    // Rysowanie punktu
    void draw(PrimitiveRenderer* renderer, float point_size = 3.0f) const;

    // Operacje matematyczne
    Point2D operator+(const Point2D& other) const {
        return Point2D(x + other.x, y + other.y);
    }

    Point2D operator-(const Point2D& other) const {
        return Point2D(x - other.x, y - other.y);
    }
};

#endif
