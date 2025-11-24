#include "PointShape.h"
#include "PrimitiveRenderer.h"
#include <allegro5/allegro_primitives.h>

void PointShape::draw(PrimitiveRenderer* renderer) const {
    if (!renderer) return;

    // Minimalna implementacja � bia�a kropka Allegro
    al_draw_filled_circle(point.getX(), point.getY(), pointSize, al_map_rgb(255,255,255));
}
