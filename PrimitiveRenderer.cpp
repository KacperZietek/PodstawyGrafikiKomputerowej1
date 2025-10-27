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

    // Obliczamy liczbe kroków
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
