#ifndef POINTSHAPE_H
#define POINTSHAPE_H

#include "ShapeObject.h"
#include "Point2D.h"
#include <cmath>

class PrimitiveRenderer;

class PointShape : public ShapeObject {
private:
    Point2D point;
    float pointSize;

public:
    PointShape(float x = 0.0f, float y = 0.0f, float size = 5.0f)
        : point(x, y), pointSize(size) {}

    virtual ~PointShape() = default;

    // Deklaracja � implementacja w PointShape.cpp
    void draw(PrimitiveRenderer* renderer) const override;

    // ========== TRANSFORMACJE ==========
    void translate(float dx, float dy) override {
        point.setPosition(point.getX() + dx, point.getY() + dy);
    }

    void rotate(float angle_deg, float cx = 0.0f, float cy = 0.0f) override {
        float rad = angle_deg * 3.14159265358979323846f / 180.0f;
        float s = std::sin(rad);
        float c = std::cos(rad);

        float x = point.getX() - cx;
        float y = point.getY() - cy;

        float xr = x * c - y * s;
        float yr = x * s + y * c;

        point.setPosition(xr + cx, yr + cy);
    }

    void scale(float sx, float sy, float cx = 0.0f, float cy = 0.0f) override {
        float x = cx + (point.getX() - cx) * sx;
        float y = cy + (point.getY() - cy) * sy;
        point.setPosition(x, y);
    }
};

#endif // POINTSHAPE_H
