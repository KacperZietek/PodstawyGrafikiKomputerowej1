#include "LineShape.h"
#include "PrimitiveRenderer.h"

void LineShape::draw(PrimitiveRenderer* renderer) const {
    if (!renderer) return;
    renderer->drawLine(
        seg.getStartX(), seg.getStartY(),
        seg.getEndX(), seg.getEndY()
    );
}
