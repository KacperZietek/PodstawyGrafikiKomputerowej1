#ifndef GAMEINTERFACES_H
#define GAMEINTERFACES_H

#include "PrimitiveRenderer.h"

// Klasa bazowa
class GameObject {
public:
    virtual ~GameObject() {}
};

// Obiekty z logik¹ (update)
class UpdatableObject : public virtual GameObject {
public:
    virtual void update(float deltaTime) = 0;
};

// Obiekty rysowalne
class DrawableObject : public virtual GameObject {
public:
    virtual void draw(PrimitiveRenderer* renderer) = 0;
};

// Obiekty transformowalne
class TransformableObject : public virtual GameObject {
public:
    virtual void translate(float dx, float dy) = 0;
    virtual void rotate(float angle, float ox, float oy) = 0;
    virtual void scale(float k, float ox, float oy) = 0;
};

// Obiekty animowane
class AnimatedObject : public virtual GameObject {
public:
    virtual void animate(float deltaTime) = 0;
};

#endif
