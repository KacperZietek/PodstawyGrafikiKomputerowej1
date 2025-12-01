#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Point2D.h"

class LineSegment {
private:
    Point2D start;
    Point2D end;

public:
    LineSegment(Point2D s, Point2D e) : start(s), end(e) {}

    Point2D getStart() const { return start; }
    Point2D getEnd() const { return end; }

    void setStart(Point2D s) { start = s; }
    void setEnd(Point2D e) { end = e; }
};

#endif
