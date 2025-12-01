#include "PrimitiveRenderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>


bool PrimitiveRenderer::areColorsEqual(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2) {
    unsigned char r1, g1, b1, a1;
    unsigned char r2, g2, b2, a2;
    al_unmap_rgba(c1, &r1, &g1, &b1, &a1);
    al_unmap_rgba(c2, &r2, &g2, &b2, &a2);
    return r1 == r2 && g1 == g2 && b1 == b2 && a1 == a2;
}

void PrimitiveRenderer::drawPoint(int x, int y, ALLEGRO_COLOR color) {
    al_draw_filled_circle(x, y, 1.5f, color);
}

void PrimitiveRenderer::drawLine(const Point2D& p1, const Point2D& p2, ALLEGRO_COLOR color) {
    al_draw_line(p1.getX(), p1.getY(), p2.getX(), p2.getY(), color, 1.0f);
}

//algorytm przyrostowy
void PrimitiveRenderer::drawLineIncremental(const Point2D& p1, const Point2D& p2, ALLEGRO_COLOR color) {
    float x1 = p1.getX();
    float y1 = p1.getY();
    float x2 = p2.getX();
    float y2 = p2.getY();

    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = (std::abs(dx) > std::abs(dy)) ? std::abs(dx) : std::abs(dy);

    if (steps == 0) {
        drawPoint(std::round(x1), std::round(y1), color);
        return;
    }

    float xInc = dx / steps;
    float yInc = dy / steps;
    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        drawPoint(std::round(x), std::round(y), color);
        x += xInc;
        y += yInc;
    }
}


//rysowanie linii
void PrimitiveRenderer::drawPolyline(const std::vector<Point2D>& points, ALLEGRO_COLOR color, bool closed) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        drawLineIncremental(points[i], points[i+1], color);
    }

    if (closed) {
        drawLineIncremental(points.back(), points.front(), color);
    }
}

//wielokat jako zlamana linia
void PrimitiveRenderer::drawPolygon(const std::vector<Point2D>& points, ALLEGRO_COLOR color) {
    drawPolyline(points, color, true);
}

//prostokat jako 4 linie
void PrimitiveRenderer::drawRectangle(const Point2D& p1, float width, float height, ALLEGRO_COLOR color, bool filled) {
    if (filled) {
        al_draw_filled_rectangle(p1.getX(), p1.getY(), p1.getX() + width, p1.getY() + height, color);
    } else {
        Point2D p2(p1.getX() + width, p1.getY());
        Point2D p3(p1.getX() + width, p1.getY() + height);
        Point2D p4(p1.getX(), p1.getY() + height);

        drawLineIncremental(p1, p2, color);
        drawLineIncremental(p2, p3, color);
        drawLineIncremental(p3, p4, color);
        drawLineIncremental(p4, p1, color);
    }
}


void PrimitiveRenderer::putCirclePixels(int cx, int cy, int x, int y, ALLEGRO_COLOR color) {
    drawPoint(cx + x, cy + y, color);
    drawPoint(cx + x, cy - y, color);
    drawPoint(cx - x, cy + y, color);
    drawPoint(cx - x, cy - y, color);
    drawPoint(cx + y, cy + x, color);
    drawPoint(cx + y, cy - x, color);
    drawPoint(cx - y, cy + x, color);
    drawPoint(cx - y, cy - x, color);
}

//algorytm Bresenhama dla okregu
void PrimitiveRenderer::drawCircle(int cx, int cy, int radius, ALLEGRO_COLOR color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    putCirclePixels(cx, cy, x, y, color);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        putCirclePixels(cx, cy, x, y, color);
    }
}

void PrimitiveRenderer::putEllipsePixels(int cx, int cy, int x, int y, ALLEGRO_COLOR color) {
    drawPoint(cx + x, cy + y, color);
    drawPoint(cx - x, cy + y, color);
    drawPoint(cx + x, cy - y, color);
    drawPoint(cx - x, cy - y, color);
}

//algorytm Midpoint dla elipsy
void PrimitiveRenderer::drawEllipse(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color) {
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    d1 = (ry * ry) - (rx * rx * ry) + (0.25f * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    while (dx < dy) {
        putEllipsePixels(cx, cy, x, y, color);
        if (d1 < 0) {
            x++;
            dx += (2 * ry * ry);
            d1 += dx + (ry * ry);
        } else {
            x++;
            y--;
            dx += (2 * ry * ry);
            dy -= (2 * rx * rx);
            d1 += dx - dy + (ry * ry);
        }
    }

    d2 = ((ry * ry) * ((x + 0.5f) * (x + 0.5f))) +
         ((rx * rx) * ((y - 1) * (y - 1))) -
         (rx * rx * ry * ry);

    while (y >= 0) {
        putEllipsePixels(cx, cy, x, y, color);
        if (d2 > 0) {
            y--;
            dy -= (2 * rx * rx);
            d2 += (rx * rx) - dy;
        } else {
            y--;
            x++;
            dx += (2 * ry * ry);
            dy -= (2 * rx * rx);
            d2 += dx - dy + (rx * rx);
        }
    }
}


void PrimitiveRenderer::floodFill(int x, int y, ALLEGRO_COLOR replacementColor) {
    ALLEGRO_BITMAP* target_bitmap = al_get_target_bitmap();

    //blokowanie bitmapy
    ALLEGRO_LOCKED_REGION* region = al_lock_bitmap(target_bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
    if (!region) return;

    int width = al_get_bitmap_width(target_bitmap);
    int height = al_get_bitmap_height(target_bitmap);

    if (x < 0 || x >= width || y < 0 || y >= height) {
        al_unlock_bitmap(target_bitmap);
        return;
    }

    ALLEGRO_COLOR targetColor = al_get_pixel(target_bitmap, x, y);

    //jezeli kolor jest taki sam to nie podminia
    if (areColorsEqual(targetColor, replacementColor)) {
        al_unlock_bitmap(target_bitmap);
        return;
    }

    std::stack<Point2D> s;
    s.push(Point2D(x, y));

    while (!s.empty()) {
        Point2D p = s.top();
        s.pop();

        int px = (int)p.getX();
        int py = (int)p.getY();

        if (px < 0 || px >= width || py < 0 || py >= height) continue;

        ALLEGRO_COLOR currentColor = al_get_pixel(target_bitmap, px, py);

        if (areColorsEqual(currentColor, targetColor)) {
            al_put_pixel(px, py, replacementColor);

            s.push(Point2D(px + 1, py));
            s.push(Point2D(px - 1, py));
            s.push(Point2D(px, py + 1));
            s.push(Point2D(px, py - 1));
        }
    }

    al_unlock_bitmap(target_bitmap);
}
