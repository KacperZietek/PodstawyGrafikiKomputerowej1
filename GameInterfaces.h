#ifndef GAMEINTERFACES_H
#define GAMEINTERFACES_H

#include "PrimitiveRenderer.h"

//klasa bazowa
class GameObject {
public:
    virtual ~GameObject() {}
};

class UpdatableObject : public virtual GameObject {
public:
    virtual void update(float deltaTime) = 0;
};

//obiekty rysowalne
class DrawableObject : public virtual GameObject {
public:
    virtual void draw(PrimitiveRenderer* renderer) = 0;
};

//obiekty transformowalne
class TransformableObject : public virtual GameObject {
public:
    virtual void translate(float dx, float dy) = 0;
    virtual void rotate(float angle, float ox, float oy) = 0;
    virtual void scale(float k, float ox, float oy) = 0;
};

//obiekty animowane
class AnimatedObject : public virtual GameObject {
public:
    virtual void animate(float deltaTime) = 0;
};

#endif
