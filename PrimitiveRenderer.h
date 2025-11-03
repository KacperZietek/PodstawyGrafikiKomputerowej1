#ifndef PRIMITIVERENDERER_H
#define PRIMITIVERENDERER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Point2D.h"
#include "LineSegment.h"

class PrimitiveRenderer {
private:
    ALLEGRO_COLOR current_color;
    float point_size;

public:
    PrimitiveRenderer();

    void setColor(ALLEGRO_COLOR color);
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void setPointSize(float size);

    void drawPoint(float x, float y);
    void drawPoint(const Point2D& point);

    void drawLine(float x1, float y1, float x2, float y2, LineAlgorithm algorithm = DEFAULT);
    void drawLine(const Point2D& start, const Point2D& end, LineAlgorithm algorithm = DEFAULT);
    void drawLine(const LineSegment& segment, LineAlgorithm algorithm = DEFAULT);
    void drawLineIncremental(float x1, float y1, float x2, float y2);
    void drawLineIncremental(const Point2D& start, const Point2D& end);

    void drawPolyline(const std::vector<Point2D>& points, bool closed = false);
    void drawPolyline(const std::vector<LineSegment>& segments, bool closed = false);

    void drawRectangle(float x1, float y1, float x2, float y2, bool filled = false);
    void drawCircle(float cx, float cy, float radius, bool filled = false);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);

    void drawCircleSymmetric(float cx, float cy, float radius, bool eightFold = false);
    void drawEllipseSymmetric(float cx, float cy, float rx, float ry, bool eightFold = false);
    void drawPolygon(const std::vector<Point2D>& vertices);
};

#endif
