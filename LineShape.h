#ifndef LINESHAPE_H
#define LINESHAPE_H

#include "ShapeObject.h"
#include "LineSegment.h"
#include <cmath>   // sin, cos

class PrimitiveRenderer;

class LineShape : public ShapeObject {
private:
    LineSegment seg;

public:
    LineShape(float x1=0,float y1=0,float x2=0,float y2=0)
        : seg(x1,y1,x2,y2) {}

    virtual ~LineShape() = default;

    // Deklaracja � IMPLEMENTACJA w LineShape.cpp
    void draw(PrimitiveRenderer* renderer) const override;

    // === TRANSFORMACJE ===
    void translate(float dx, float dy) override {
        seg.setStartPosition(seg.getStartX() + dx, seg.getStartY() + dy);
        seg.setEndPosition(seg.getEndX() + dx,   seg.getEndY() + dy);
    }

    void rotate(float angle_deg, float cx = 0.0f, float cy = 0.0f) override {
        float rad = angle_deg * 3.14159265358979323846f / 180.0f;
        float s = std::sin(rad);
        float c = std::cos(rad);

        // punkt A
        float ax = seg.getStartX() - cx;
        float ay = seg.getStartY() - cy;

        float arx = ax * c - ay * s;
        float ary = ax * s + ay * c;

        // punkt B
        float bx = seg.getEndX() - cx;
        float by = seg.getEndY() - cy;

        float brx = bx * c - by * s;
        float bry = bx * s + by * c;

        // powr�t
        seg.setStartPosition(arx + cx, ary + cy);
        seg.setEndPosition(brx + cx, bry + cy);
    }

    void scale(float sx, float sy, float cx = 0.0f, float cy = 0.0f) override {
        float x1 = cx + (seg.getStartX() - cx) * sx;
        float y1 = cy + (seg.getStartY() - cy) * sy;
        float x2 = cx + (seg.getEndX()   - cx) * sx;
        float y2 = cy + (seg.getEndY()   - cy) * sy;

        seg.setStartPosition(x1, y1);
        seg.setEndPosition(x2, y2);
    }
};

#endif // LINESHAPE_H
