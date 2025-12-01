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
