#include "PrimitiveRenderer.h"
#include <cmath>
#include <algorithm>
#include <stack>
#include <queue>
#include <set>
#include <iostream>

// Konstruktor
PrimitiveRenderer::PrimitiveRenderer()
    : current_color(al_map_rgb(255, 255, 255)), point_size(3.0f) {
}

// PROSTSZE I DZIAŁAJĄCE WYPEŁNIANIE
bool PrimitiveRenderer::isValidPoint(int x, int y) {
    ALLEGRO_DISPLAY* display = al_get_current_display();
    if (!display) return false;
    return (x >= 0 && y >= 0 && x < al_get_display_width(display) && y < al_get_display_height(display));
}

bool PrimitiveRenderer::color_equal(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2) {
    unsigned char r1, g1, b1, a1, r2, g2, b2, a2;
    al_unmap_rgba(c1, &r1, &g1, &b1, &a1);
    al_unmap_rgba(c2, &r2, &g2, &b2, &a2);
    return (r1 == r2 && g1 == g2 && b1 == b2);
}

// ITERACYJNE WYPEŁNIANIE
void PrimitiveRenderer::boundaryFillIterative(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR boundary_color) {
    std::stack<std::pair<int, int>> stack;
    stack.push({x, y});

    int filled_pixels = 0;
    const int MAX_PIXELS = 10000; // Zabezpieczenie przed zawieszeniem

    while (!stack.empty() && filled_pixels < MAX_PIXELS) {
        auto current = stack.top();
        stack.pop();
        int current_x = current.first;
        int current_y = current.second;

        if (!isValidPoint(current_x, current_y)) continue;

        // Tworzymy tymczasową bitmape do sprawdzenia koloru
        ALLEGRO_BITMAP* target = al_get_target_bitmap();
        ALLEGRO_BITMAP* temp_bmp = al_create_bitmap(1, 1);
        al_set_target_bitmap(temp_bmp);
        al_draw_bitmap_region(target, current_x, current_y, 1, 1, 0, 0, 0);
        ALLEGRO_COLOR current_color = al_get_pixel(temp_bmp, 0, 0);
        al_destroy_bitmap(temp_bmp);
        al_set_target_bitmap(target);

        if (!color_equal(current_color, boundary_color) && !color_equal(current_color, fill_color)) {
            al_put_pixel(current_x, current_y, fill_color);
            filled_pixels++;

            stack.push({current_x + 1, current_y});
            stack.push({current_x - 1, current_y});
            stack.push({current_x, current_y + 1});
            stack.push({current_x, current_y - 1});
        }
    }

    std::cout << "Boundary fill: wypelniono " << filled_pixels << " pikseli" << std::endl;
}

void PrimitiveRenderer::floodFillIterative(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR old_color) {
    std::stack<std::pair<int, int>> stack;
    stack.push({x, y});

    int filled_pixels = 0;
    const int MAX_PIXELS = 10000;

    while (!stack.empty() && filled_pixels < MAX_PIXELS) {
        auto current = stack.top();
        stack.pop();
        int current_x = current.first;
        int current_y = current.second;

        if (!isValidPoint(current_x, current_y)) continue;

        // Tworzymy tymczasową bitmapę do sprawdzenia koloru
        ALLEGRO_BITMAP* target = al_get_target_bitmap();
        ALLEGRO_BITMAP* temp_bmp = al_create_bitmap(1, 1);
        al_set_target_bitmap(temp_bmp);
        al_draw_bitmap_region(target, current_x, current_y, 1, 1, 0, 0, 0);
        ALLEGRO_COLOR current_color = al_get_pixel(temp_bmp, 0, 0);
        al_destroy_bitmap(temp_bmp);
        al_set_target_bitmap(target);

        if (color_equal(current_color, old_color)) {
            al_put_pixel(current_x, current_y, fill_color);
            filled_pixels++;

            stack.push({current_x + 1, current_y});
            stack.push({current_x - 1, current_y});
            stack.push({current_x, current_y + 1});
            stack.push({current_x, current_y - 1});
        }
    }

    std::cout << "Flood fill: wypełniono " << filled_pixels << " pikseli" << std::endl;
}

// GŁOWNA METODA WYPEŁNIANIA
void PrimitiveRenderer::fillArea(int x, int y, ALLEGRO_COLOR fill_color, ALLEGRO_COLOR boundary_or_old_color,
                               FillAlgorithm algorithm, bool recursive) {

    std::cout << "Wypełnianie obszaru w punkcie: " << x << ", " << y << std::endl;
    std::cout << "Kolor wypełnienia: R=" << (int)(fill_color.r * 255) << " G=" << (int)(fill_color.g * 255) << " B=" << (int)(fill_color.b * 255) << std::endl;

    // Zawsze używamy wersji iteracyjnej - jest bezpieczniejsza
    if (algorithm == FLOOD_FILL) {
        floodFillIterative(x, y, fill_color, boundary_or_old_color);
    } else { // BOUNDARY_FILL
        boundaryFillIterative(x, y, fill_color, boundary_or_old_color);
    }

    std::cout << "Wypełnianie zakończone" << std::endl;
}

// POZOSTAŁE METODY
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
    if (algorithm == INCREMENTAL)
        drawLineIncremental(x1, y1, x2, y2);
    else
        al_draw_line(x1, y1, x2, y2, current_color, 1.0f);
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
    int steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0) {
        drawPoint(x1, y1);
        return;
    }

    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
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
    for (size_t i = 0; i < points.size() - 1; i++)
        drawLine(points[i], points[i + 1]);
    if (closed && points.size() > 2)
        drawLine(points.back(), points.front());
}

void PrimitiveRenderer::drawPolyline(const std::vector<LineSegment>& segments, bool closed) {
    for (const auto& s : segments)
        drawLine(s);
    if (closed && segments.size() > 1)
        drawLine(segments.back().getEnd(), segments.front().getStart());
}

void PrimitiveRenderer::drawRectangle(float x1, float y1, float x2, float y2, bool filled) {
    if (filled)
        al_draw_filled_rectangle(x1, y1, x2, y2, current_color);
    else
        al_draw_rectangle(x1, y1, x2, y2, current_color, 1.0f);
}

void PrimitiveRenderer::drawCircle(float cx, float cy, float radius, bool filled) {
    if (filled)
        al_draw_filled_circle(cx, cy, radius, current_color);
    else
        al_draw_circle(cx, cy, radius, current_color, 1.0f);
}

void PrimitiveRenderer::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled) {
    if (filled)
        al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, current_color);
    else
        al_draw_triangle(x1, y1, x2, y2, x3, y3, current_color, 1.0f);
}

void PrimitiveRenderer::drawEllipse(float cx, float cy, float rx, float ry, bool filled) {
    if (filled)
        al_draw_filled_ellipse(cx, cy, rx, ry, current_color);
    else
        al_draw_ellipse(cx, cy, rx, ry, current_color, 1.0f);
}

void PrimitiveRenderer::drawCircleSymmetric(float cx, float cy, float radius, bool eightFold) {
    float step = (eightFold ? ALLEGRO_PI / 4 : ALLEGRO_PI / 2) / radius;

    for (float alpha = 0; alpha <= (eightFold ? ALLEGRO_PI / 4 : ALLEGRO_PI / 2); alpha += step) {
        float x = radius * cos(alpha);
        float y = radius * sin(alpha);

        drawPoint(cx + x, cy + y);
        drawPoint(cx - x, cy + y);
        drawPoint(cx + x, cy - y);
        drawPoint(cx - x, cy - y);

        if (eightFold) {
            drawPoint(cx + y, cy + x);
            drawPoint(cx - y, cy + x);
            drawPoint(cx + y, cy - x);
            drawPoint(cx - y, cy - x);
        }
    }
}

void PrimitiveRenderer::drawEllipseSymmetric(float cx, float cy, float rx, float ry, bool eightFold) {
    float step = (eightFold ? ALLEGRO_PI / 4 : ALLEGRO_PI / 2) / std::max(rx, ry);

    for (float alpha = 0; alpha <= (eightFold ? ALLEGRO_PI / 4 : ALLEGRO_PI / 2); alpha += step) {
        float x = rx * cos(alpha);
        float y = ry * sin(alpha);

        drawPoint(cx + x, cy + y);
        drawPoint(cx - x, cy + y);
        drawPoint(cx + x, cy - y);
        drawPoint(cx - x, cy - y);

        if (eightFold) {
            drawPoint(cx + y * (rx / ry), cy + x * (ry / rx));
            drawPoint(cx - y * (rx / ry), cy + x * (ry / rx));
            drawPoint(cx + y * (rx / ry), cy - x * (ry / rx));
            drawPoint(cx - y * (rx / ry), cy - x * (ry / rx));
        }
    }
}

void PrimitiveRenderer::drawPolygon(const std::vector<Point2D>& vertices) {
    if (vertices.size() < 3) return;
    for (size_t i = 0; i < vertices.size(); ++i)
        drawLine(vertices[i], vertices[(i + 1) % vertices.size()]);
}

void PrimitiveRenderer::fillPolygon(const std::vector<Point2D>& vertices) {
    if (vertices.size() < 3) return;

    // Uzywamy wbudowanej funkcji Allegro do wypelniania wielokatow
    std::vector<float> points;
    for (const auto& v : vertices) {
        points.push_back(v.getX());
        points.push_back(v.getY());
    }
    al_draw_filled_polygon(points.data(), points.size() / 2, current_color);
}
