#include "LineSegment.h"
#include "PrimitiveRenderer.h"
#include <cmath>

void LineSegment::draw(PrimitiveRenderer* renderer, LineAlgorithm algorithm) const {
    if (renderer) {
        renderer->drawLine(start, end, algorithm);
    }
}

float LineSegment::length() const {
    float dx = end.getX() - start.getX();
    float dy = end.getY() - start.getY();
    return std::sqrt(dx * dx + dy * dy);
}
