<<<<<<< Updated upstream
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

    // Ustawienia
    void setColor(ALLEGRO_COLOR color);
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void setPointSize(float size);

    // Podstawowe prymitywy
    void drawPoint(float x, float y);
    void drawPoint(const Point2D& point);

    void drawLine(float x1, float y1, float x2, float y2, LineAlgorithm algorithm = DEFAULT);
    void drawLine(const Point2D& start, const Point2D& end, LineAlgorithm algorithm = DEFAULT);
    void drawLine(const LineSegment& segment, LineAlgorithm algorithm = DEFAULT);

    // Algorytm przyrostowy
    void drawLineIncremental(float x1, float y1, float x2, float y2);
    void drawLineIncremental(const Point2D& start, const Point2D& end);

    // Linie lamane
    void drawPolyline(const std::vector<Point2D>& points, bool closed = false);
    void drawPolyline(const std::vector<LineSegment>& segments, bool closed = false);

    // Inne prymitywy
    void drawRectangle(float x1, float y1, float x2, float y2, bool filled = false);
    void drawCircle(float cx, float cy, float radius, bool filled = false);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);

    // NOWE METODY - w³asne algorytmy
    void drawCircleBresenham(float cx, float cy, float radius, bool filled = false);
    void drawEllipseMidpoint(float cx, float cy, float rx, float ry, bool filled = false);

private:
    // Pomocnicze metody dla algorytmów
    void drawCirclePoints(float cx, float cy, float x, float y, bool filled);
    void drawEllipsePoints(float cx, float cy, float x, float y);
};

#endif
=======
#ifndef PRIMITIVERENDERER_H
#define PRIMITIVERENDERER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <stack>
#include <cmath>
#include "Point2D.h"

class PrimitiveRenderer {
public:
    // Rysowanie punktu (pomocnicze)
    void drawPoint(int x, int y, ALLEGRO_COLOR color);

    // Rysowanie linii (Biblioteka vs Algorytm przyrostowy)
    void drawLine(const Point2D& p1, const Point2D& p2, ALLEGRO_COLOR color);
    void drawLineIncremental(const Point2D& p1, const Point2D& p2, ALLEGRO_COLOR color);

    // --- TO BY£O BRAKUJ¥CE (Naprawa b³êdu) ---
    void drawRectangle(const Point2D& p1, float width, float height, ALLEGRO_COLOR color, bool filled = false);

    // Linie ³amane i Wielok¹ty
    void drawPolyline(const std::vector<Point2D>& points, ALLEGRO_COLOR color, bool closed = false);
    void drawPolygon(const std::vector<Point2D>& points, ALLEGRO_COLOR color);

    // Okrêgi i Elipsy (Lab 03)
    void drawCircle(int cx, int cy, int radius, ALLEGRO_COLOR color);
    void drawEllipse(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color);

    // Wype³nianie (Flood Fill - Lab 03)
    void floodFill(int x, int y, ALLEGRO_COLOR replacementColor);

private:
    // Metody pomocnicze do symetrii
    void putCirclePixels(int cx, int cy, int x, int y, ALLEGRO_COLOR color);
    void putEllipsePixels(int cx, int cy, int x, int y, ALLEGRO_COLOR color);

    // Pomocnicze do kolorów
    bool areColorsEqual(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2);
};

#endif
>>>>>>> Stashed changes
