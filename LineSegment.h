#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Point2D.h"


enum LineAlgorithm {
    DEFAULT,
    INCREMENTAL
};

class PrimitiveRenderer;

class LineSegment {
private:
    Point2D start;
    Point2D end;

public:
    LineSegment(const Point2D& start = Point2D(), const Point2D& end = Point2D())
        : start(start), end(end) {}

    LineSegment(float x1, float y1, float x2, float y2)
        : start(x1, y1), end(x2, y2) {}

    // Gettery
    Point2D getStart() const { return start; }
    Point2D getEnd() const { return end; }
    float getStartX() const { return start.getX(); }
    float getStartY() const { return start.getY(); }
    float getEndX() const { return end.getX(); }
    float getEndY() const { return end.getY(); }

    // Settery
    void setStart(const Point2D& newStart) { start = newStart; }
    void setEnd(const Point2D& newEnd) { end = newEnd; }
    void setStartPosition(float x, float y) { start.setPosition(x, y); }
    void setEndPosition(float x, float y) { end.setPosition(x, y); }

    // Rysowanie odcinka
    void draw(PrimitiveRenderer* renderer, LineAlgorithm algorithm = DEFAULT) const;

    // Dlugosc odcinka
    float length() const;
};

#endif
