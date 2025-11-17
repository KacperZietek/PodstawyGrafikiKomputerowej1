#ifndef PRIMITIVERENDERER_H
#define PRIMITIVERENDERER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include "Point2D.h"
#include "LineSegment.h"

enum FillAlgorithm {
    FLOOD_FILL,
    BOUNDARY_FILL
};

enum Neighborhood {
    VON_NEUMANN,
    MOORE
};

class PrimitiveRenderer {
private:
    ALLEGRO_COLOR current_color;
    float point_size;

    // POMOCNICZE METODY DLA WYPE£NIANIA
    bool isValidPoint(int x, int y);
    bool color_equal(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2);
    ALLEGRO_COLOR getPixelColor(int x, int y);
    void setPixelColor(int x, int y, ALLEGRO_COLOR color);

    void boundaryFill4(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR boundary_color);
    void floodFill4(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR old_color);
    void boundaryFillIterative(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR boundary_color);
    void floodFillIterative(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR old_color);

public:
    PrimitiveRenderer();

    void setColor(ALLEGRO_COLOR color);
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void setPointSize(float size);

    // Punkty
    void drawPoint(float x, float y);
    void drawPoint(const Point2D& point);

    // Linie
    void drawLine(float x1, float y1, float x2, float y2, LineAlgorithm algorithm = DEFAULT);
    void drawLine(const Point2D& start, const Point2D& end, LineAlgorithm algorithm = DEFAULT);
    void drawLine(const LineSegment& segment, LineAlgorithm algorithm = DEFAULT);
    void drawLineIncremental(float x1, float y1, float x2, float y2);
    void drawLineIncremental(const Point2D& start, const Point2D& end);

    // £amane
    void drawPolyline(const std::vector<Point2D>& points, bool closed = false);
    void drawPolyline(const std::vector<LineSegment>& segments, bool closed = false);

    // Figury
    void drawRectangle(float x1, float y1, float x2, float y2, bool filled = false);
    void drawCircle(float cx, float cy, float radius, bool filled = false);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);
    void drawEllipse(float cx, float cy, float rx, float ry, bool filled = false);

    // Symetryczne algorytmy
    void drawCircleSymmetric(float cx, float cy, float radius, bool eightFold = false);
    void drawEllipseSymmetric(float cx, float cy, float rx, float ry, bool eightFold = false);

    // Wielokaty
    void drawPolygon(const std::vector<Point2D>& vertices);
    void fillPolygon(const std::vector<Point2D>& vertices);

    // WYPE£NIANIE OBSZARU
    void fillArea(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR boundary_or_old_color,
                 FillAlgorithm algorithm = FLOOD_FILL, bool recursive = true);
};

#endif
