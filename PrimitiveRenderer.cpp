<<<<<<< Updated upstream
#include "PrimitiveRenderer.h"
#include <cmath>
#include <algorithm>

PrimitiveRenderer::PrimitiveRenderer()
    : current_color(al_map_rgb(255, 255, 255)), point_size(3.0f) {
}

void PrimitiveRenderer::setColor(ALLEGRO_COLOR color) {
    current_color = color;
}

void PrimitiveRenderer::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    current_color = al_map_rgba(r, g, b, a);
}

void PrimitiveRenderer::setPointSize(float size) {
    point_size = size;
}

void PrimitiveRenderer::drawPoint(float x, float y) {
    al_draw_filled_circle(x, y, point_size, current_color);
}

void PrimitiveRenderer::drawPoint(const Point2D& point) {
    drawPoint(point.getX(), point.getY());
}

void PrimitiveRenderer::drawLine(float x1, float y1, float x2, float y2, LineAlgorithm algorithm) {
    if (algorithm == INCREMENTAL) {
        drawLineIncremental(x1, y1, x2, y2);
    } else {
        al_draw_line(x1, y1, x2, y2, current_color, 1.0f);
    }
}

void PrimitiveRenderer::drawLine(const Point2D& start, const Point2D& end, LineAlgorithm algorithm) {
    drawLine(start.getX(), start.getY(), end.getX(), end.getY(), algorithm);
}

void PrimitiveRenderer::drawLine(const LineSegment& segment, LineAlgorithm algorithm) {
    drawLine(segment.getStart(), segment.getEnd(), algorithm);
}

void PrimitiveRenderer::drawLineIncremental(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;

    // Obliczamy liczbe krokÃ³w
    int steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0) {
        drawPoint(x1, y1);
        return;
    }

    // Obliczamy przyrosty
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    // Rysujemy linie punkt po punkcie
    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        drawPoint(std::round(x), std::round(y));
        x += xIncrement;
        y += yIncrement;
    }
}

void PrimitiveRenderer::drawLineIncremental(const Point2D& start, const Point2D& end) {
    drawLineIncremental(start.getX(), start.getY(), end.getX(), end.getY());
}

void PrimitiveRenderer::drawPolyline(const std::vector<Point2D>& points, bool closed) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; i++) {
        drawLine(points[i], points[i + 1]);
    }

    if (closed && points.size() > 2) {
        drawLine(points.back(), points.front());
    }
}

void PrimitiveRenderer::drawPolyline(const std::vector<LineSegment>& segments, bool closed) {
    for (const auto& segment : segments) {
        drawLine(segment);
    }

    if (closed && segments.size() > 1) {
        drawLine(segments.back().getEnd(), segments.front().getStart());
    }
}

void PrimitiveRenderer::drawRectangle(float x1, float y1, float x2, float y2, bool filled) {
    if (filled) {
        al_draw_filled_rectangle(x1, y1, x2, y2, current_color);
    } else {
        al_draw_rectangle(x1, y1, x2, y2, current_color, 1.0f);
    }
}

void PrimitiveRenderer::drawCircle(float cx, float cy, float radius, bool filled) {
    if (filled) {
        al_draw_filled_circle(cx, cy, radius, current_color);
    } else {
        al_draw_circle(cx, cy, radius, current_color, 1.0f);
    }
}

void PrimitiveRenderer::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled) {
    if (filled) {
        al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, current_color);
    } else {
        al_draw_triangle(x1, y1, x2, y2, x3, y3, current_color, 1.0f);
    }
}

// Rysuje 8 punktÃ³w wykorzystujÄ…c symetriÄ™ okrÄ™gu
void PrimitiveRenderer::drawCirclePoints(float cx, float cy, float x, float y, bool filled) {
    if (filled) {
        // Dla wypeÅ‚nionego okrÄ™gu - rysujemy poziome linie
        al_draw_line(cx - x, cy + y, cx + x, cy + y, current_color, 1.0f);
        al_draw_line(cx - x, cy - y, cx + x, cy - y, current_color, 1.0f);
        al_draw_line(cx - y, cy + x, cx + y, cy + x, current_color, 1.0f);
        al_draw_line(cx - y, cy - x, cx + y, cy - x, current_color, 1.0f);
    } else {
        // Dla obramowanego okrÄ™gu - rysujemy 8 punktÃ³w
        drawPoint(cx + x, cy + y);
        drawPoint(cx - x, cy + y);
        drawPoint(cx + x, cy - y);
        drawPoint(cx - x, cy - y);
        drawPoint(cx + y, cy + x);
        drawPoint(cx - y, cy + x);
        drawPoint(cx + y, cy - x);
        drawPoint(cx - y, cy - x);
    }
}

// Algorytm Bresenhama dla okrÄ™gu
void PrimitiveRenderer::drawCircleBresenham(float cx, float cy, float radius, bool filled) {
    if (radius <= 0) return;

    int x = 0;
    int y = static_cast<int>(radius);
    int d = 3 - 2 * static_cast<int>(radius);

    drawCirclePoints(cx, cy, x, y, filled);

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        drawCirclePoints(cx, cy, x, y, filled);
    }
}

// Rysuje 4 punkty wykorzystujÄ…c symetriÄ™ elipsy
void PrimitiveRenderer::drawEllipsePoints(float cx, float cy, float x, float y) {
    drawPoint(cx + x, cy + y);
    drawPoint(cx - x, cy + y);
    drawPoint(cx + x, cy - y);
    drawPoint(cx - x, cy - y);
}

// Algorytm midpoint dla elipsy
void PrimitiveRenderer::drawEllipseMidpoint(float cx, float cy, float rx, float ry, bool filled) {
    if (rx <= 0 || ry <= 0) return;

    float rx2 = rx * rx;
    float ry2 = ry * ry;
    float twoRx2 = 2 * rx2;
    float twoRy2 = 2 * ry2;

    float x = 0;
    float y = ry;
    float px = 0;
    float py = twoRx2 * y;

    // Region 1
    float p = ry2 - (rx2 * ry) + (0.25f * rx2);
    while (px < py) {
        x++;
        px += twoRy2;

        if (p < 0) {
            p += ry2 + px;
        } else {
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }

        if (filled) {
            // Dla wypeÅ‚nionej elipsy - rysujemy poziome linie
            al_draw_line(cx - x, cy + y, cx + x, cy + y, current_color, 1.0f);
            al_draw_line(cx - x, cy - y, cx + x, cy - y, current_color, 1.0f);
        } else {
            drawEllipsePoints(cx, cy, x, y);
        }
    }

    // Region 2
    p = ry2 * (x + 0.5f) * (x + 0.5f) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    while (y > 0) {
        y--;
        py -= twoRx2;

        if (p > 0) {
            p += rx2 - py;
        } else {
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }

        if (filled) {
            al_draw_line(cx - x, cy + y, cx + x, cy + y, current_color, 1.0f);
            al_draw_line(cx - x, cy - y, cx + x, cy - y, current_color, 1.0f);
        } else {
            drawEllipsePoints(cx, cy, x, y);
        }
    }
}
=======
#include "PrimitiveRenderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// ---------------------------------------------------------
// HELPERY I USTAWIENIA
// ---------------------------------------------------------

bool PrimitiveRenderer::areColorsEqual(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2) {
    unsigned char r1, g1, b1, a1;
    unsigned char r2, g2, b2, a2;
    al_unmap_rgba(c1, &r1, &g1, &b1, &a1);
    al_unmap_rgba(c2, &r2, &g2, &b2, &a2);
    return r1 == r2 && g1 == g2 && b1 == b2 && a1 == a2;
}

// ---------------------------------------------------------
// PODSTAWOWE PRYMITYWY (PUNKT, LINIA)
// ---------------------------------------------------------

void PrimitiveRenderer::drawPoint(int x, int y, ALLEGRO_COLOR color) {
    al_draw_filled_circle(x, y, 1.5f, color);
}

void PrimitiveRenderer::drawLine(const Point2D& p1, const Point2D& p2, ALLEGRO_COLOR color) {
    // U¿ywa wbudowanej funkcji Allegro (dla porównania)
    al_draw_line(p1.getX(), p1.getY(), p2.getX(), p2.getY(), color, 1.0f);
}

// ALGORYTM PRZYROSTOWY (LAB 02)
void PrimitiveRenderer::drawLineIncremental(const Point2D& p1, const Point2D& p2, ALLEGRO_COLOR color) {
    float x1 = p1.getX();
    float y1 = p1.getY();
    float x2 = p2.getX();
    float y2 = p2.getY();

    float dx = x2 - x1;
    float dy = y2 - y1;

    // Wybieramy wiêksz¹ ró¿nicê jako liczbê kroków
    float steps = (std::abs(dx) > std::abs(dy)) ? std::abs(dx) : std::abs(dy);

    // Zapobiegamy dzieleniu przez zero (gdy punkty s¹ w tym samym miejscu)
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

// ---------------------------------------------------------
// LINIE £AMANE I WIELOK¥TY (LAB 02 i 03)
// ---------------------------------------------------------

// Rysowanie linii ³amanej (otwartej lub zamkniêtej)
void PrimitiveRenderer::drawPolyline(const std::vector<Point2D>& points, ALLEGRO_COLOR color, bool closed) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        drawLineIncremental(points[i], points[i+1], color);
    }

    if (closed) {
        drawLineIncremental(points.back(), points.front(), color);
    }
}

// Wielok¹t to po prostu zamkniêta linia ³amana
void PrimitiveRenderer::drawPolygon(const std::vector<Point2D>& points, ALLEGRO_COLOR color) {
    drawPolyline(points, color, true);
}

// Prostok¹t jako 4 linie
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

// ---------------------------------------------------------
// OKRÊGI I ELIPSY (LAB 03)
// ---------------------------------------------------------

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

// Algorytm Bresenhama dla okrêgu
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

// Algorytm Midpoint dla elipsy
void PrimitiveRenderer::drawEllipse(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color) {
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    // Region 1
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

    // Region 2
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

// ---------------------------------------------------------
// WYPE£NIANIE (LAB 03 - FLOOD FILL)
// ---------------------------------------------------------

void PrimitiveRenderer::floodFill(int x, int y, ALLEGRO_COLOR replacementColor) {
    ALLEGRO_BITMAP* target_bitmap = al_get_target_bitmap();

    // Blokowanie bitmapy dla wydajnoœci
    ALLEGRO_LOCKED_REGION* region = al_lock_bitmap(target_bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
    if (!region) return;

    int width = al_get_bitmap_width(target_bitmap);
    int height = al_get_bitmap_height(target_bitmap);

    if (x < 0 || x >= width || y < 0 || y >= height) {
        al_unlock_bitmap(target_bitmap);
        return;
    }

    ALLEGRO_COLOR targetColor = al_get_pixel(target_bitmap, x, y);

    // Jeœli kolor jest taki sam, nie ma co robiæ
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
>>>>>>> Stashed changes
