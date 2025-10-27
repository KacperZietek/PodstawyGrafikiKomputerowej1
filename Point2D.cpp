#include "Point2D.h"
#include "PrimitiveRenderer.h"

void Point2D::draw(PrimitiveRenderer* renderer, float point_size) const {
    if (renderer) {
        float old_size = 3.0f; // Zapamietaj startowo wielkosc
        renderer->setPointSize(point_size);
        renderer->drawPoint(x, y);
        renderer->setPointSize(old_size); // Przywróc startowo wielkosc
    }
}
