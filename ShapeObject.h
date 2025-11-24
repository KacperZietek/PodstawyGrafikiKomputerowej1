#ifndef SHAPEOBJECT_H
#define SHAPEOBJECT_H

#include "DrawableObject.h"
#include "TransformableObject.h"

// ShapeObject dziedziczy po DrawableObject i TransformableObject
class ShapeObject : public DrawableObject, public TransformableObject {
public:
    virtual ~ShapeObject() = default;
    // dodatkowe API shape (opcjonalne) - bbox, hit test etc.
};

#endif // SHAPEOBJECT_H
