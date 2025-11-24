#ifndef TRANSFORMABLEOBJECT_H
#define TRANSFORMABLEOBJECT_H

#include "GameObject.h"

// Przekszta³cenia 2D: translate(dx,dy), rotate(angle_deg, cx, cy), scale(sx, sy, cx, cy)
class TransformableObject : virtual public GameObject {
public:
    virtual ~TransformableObject() = default;
    virtual void translate(float dx, float dy) = 0;
    // angle in degrees, center cx,cy
    virtual void rotate(float angle_deg, float cx = 0.0f, float cy = 0.0f) = 0;
    virtual void scale(float sx, float sy, float cx = 0.0f, float cy = 0.0f) = 0;
};

#endif // TRANSFORMABLEOBJECT_H
