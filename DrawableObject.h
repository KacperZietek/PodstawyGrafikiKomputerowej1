#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "GameObject.h"

class PrimitiveRenderer;

class DrawableObject : virtual public GameObject {
public:
    virtual ~DrawableObject() = default;
    // renderer mo¿e byæ nullptr
    virtual void draw(PrimitiveRenderer* renderer) const = 0;
};

#endif // DRAWABLEOBJECT_H abc
//
